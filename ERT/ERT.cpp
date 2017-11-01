//
//  ERT.cpp
//  ERT
//
//  Created by Mamunul on 11/1/17.
//  Copyright © 2017 Mamunul. All rights reserved.
//

#include "ERT.hpp"

Params global_params;
std::string folderPath = "";
std::string modelPath ="/model_69/";
std::string dataPath = "/Dataset/";
std::string cascadeName = "haarcascade_frontalface_alt.xml";

using namespace std;
using namespace dlib;

using namespace cv;

double interocular_distance (
							 const full_object_detection& det
							 )
{
	dlib::vector<double,2> l, r;
	double cnt = 0;
	// Find the center of the left eye by averaging the points around
	// the eye.
	for (unsigned long i = 36; i <= 41; ++i)
	{
		l += det.part(i);
		++cnt;
	}
	l /= cnt;
	
	// Find the center of the right eye by averaging the points around
	// the eye.
	cnt = 0;
	for (unsigned long i = 42; i <= 47; ++i)
	{
		r += det.part(i);
		++cnt;
	}
	r /= cnt;
	
	// Now return the distance between the centers of the eyes
	return length(l-r);
}

std::vector<std::vector<double> > get_interocular_distances (
															 const std::vector<std::vector<full_object_detection> >& objects
															 )
{
	std::vector<std::vector<double> > temp(objects.size());
	for (unsigned long i = 0; i < objects.size(); ++i)
	{
		for (unsigned long j = 0; j < objects[i].size(); ++j)
		{
			temp[i].push_back(interocular_distance(objects[i][j]));
		}
	}
	return temp;
}
std::vector<point> LoadGroundTruthShape(string& filename){
	Mat_<double> shape(global_params.landmark_num,2);
	std::vector<point> partlist(global_params.landmark_num, OBJECT_PART_NOT_PRESENT);
	ifstream fin;
	string temp;
	
	fin.open(filename);
	getline(fin, temp);
	getline(fin, temp);
	getline(fin, temp);
	for (int i=0;i<global_params.landmark_num;i++){
		double a,b;
		fin >> a >> b;
		
		point p(a,b);

		partlist.push_back(p);
	}
	fin.close();
	return partlist;
	
}

bool IsShapeInRect(std::vector<point> partlist, Rect& rect,double scale){
	double sum1 = 0;
	double sum2 = 0;
	double max_x=0,min_x=10000,max_y=0,min_y=10000;
	for (int i= 0;i < partlist.size();i++){
		if (partlist[i].x()>max_x) max_x = partlist[i].x();
		if (partlist[i].x()<min_x) min_x = partlist[i].x();
		if (partlist[i].y()>max_y) max_y = partlist[i].y();
		if (partlist[i].y()<min_y) min_y = partlist[i].y();
		
		sum1 += partlist[i].x();
		sum2 += partlist[i].y();
	}
	if ((max_x-min_x)>rect.width*1.5){
		return false;
	}
	if ((max_y-min_y)>rect.height*1.5){
		return false;
	}
	if (abs(sum1/partlist.size() - (rect.x+rect.width/2.0)*scale) > rect.width*scale/2.0){
		return false;
	}
	if (abs(sum2/partlist.size() - (rect.y+rect.height/2.0)*scale) > rect.height*scale/2.0){
		return false;
	}
	return true;
}

void adjustImage(Mat_<uchar>& img,
				 Mat_<double>& ground_truth_shape,
				 BoundingBox& bounding_box){
	double left_x  = max(1.0, bounding_box.centroid_x - bounding_box.width*2/3);
	double top_y   = max(1.0, bounding_box.centroid_y - bounding_box.height*2/3);
	double right_x = min(img.cols-1.0,bounding_box.centroid_x+bounding_box.width);
	double bottom_y= min(img.rows-1.0,bounding_box.centroid_y+bounding_box.height);
	img = img.rowRange((int)top_y,(int)bottom_y).colRange((int)left_x,(int)right_x).clone();
	
	bounding_box.start_x = bounding_box.start_x-left_x;
	bounding_box.start_y = bounding_box.start_y-top_y;
	bounding_box.centroid_x = bounding_box.start_x + bounding_box.width/2.0;
	bounding_box.centroid_y = bounding_box.start_y + bounding_box.height/2.0;
	
	for(int i=0;i<ground_truth_shape.rows;i++){
		ground_truth_shape(i,0) = ground_truth_shape(i,0)-left_x;
		ground_truth_shape(i,1) = ground_truth_shape(i,1)-top_y;
	}
}

void LoadOpencvBbxData(string _folderPath,string filepath,dlib::array<array2d<unsigned char> >& images, std::vector<std::vector<full_object_detection> >  & ground_truth_shapes){

	//	printf("path:%s,%s\n",folderPath.c_str(),filepath.c_str());dlib::cv_image<unsigned char>(image)
	ifstream fin;
	fin.open(filepath);
	
	CascadeClassifier cascade;
	double scale = 1.3;
	extern string cascadeName;
	std::vector<cv::Rect> faces;
	cv::Mat gray;
	
	cascadeName = folderPath+cascadeName;
	
	// --Detection
	cascade.load(cascadeName);
	
	//	printf("cas:%s\n",cascadeName.c_str());
	string name;
	while(getline(fin,name)){
		name.erase(0, name.find_first_not_of(" \t"));
		name.erase(name.find_last_not_of(" \t") + 1);
		//        cout << "file:" << folderPath+"/"+name <<endl;
		// Read Image
		
		name = _folderPath + "/" + name;
		Mat_<uchar> image = imread(name,0);
		
		
		// Read ground truth shapes
		name.replace(name.find_last_of("."), 4,".pts");
		std::vector<point> partlist = LoadGroundTruthShape(name);
		
		// Read OPencv Detection Bbx
		Mat smallImg( cvRound (image.rows/scale), cvRound(image.cols/scale), CV_8UC1 );
		resize( image, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
		equalizeHist( smallImg, smallImg );
		
		// --Detection
		cascade.detectMultiScale( smallImg, faces,
								 1.1, 2, 0
								 //|CV_HAAR_FIND_BIGGEST_OBJECT
								 //|CV_HAAR_DO_ROUGH_SEARCH
								 |CV_HAAR_SCALE_IMAGE
								 ,
								 Size(30, 30) );
		
		std::vector<full_object_detection> shapePerImage;
		
		
		for( std::vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++){
			Rect rect = *r;
			if (IsShapeInRect(partlist,rect,scale)){
				Point center;
				BoundingBox boundingbox;
				
				boundingbox.start_x = r->x*scale;
				boundingbox.start_y = r->y*scale;
				boundingbox.width   = (r->width-1)*scale;
				boundingbox.height  = (r->height-1)*scale;
				boundingbox.centroid_x = boundingbox.start_x + boundingbox.width/2.0;
				boundingbox.centroid_y = boundingbox.start_y + boundingbox.height/2.0;
				
				
//				adjustImage(image,ground_truth_shape,boundingbox);
				
				dlib::array2d<unsigned char> dlibImageGray;
				dlib::assign_image(dlibImageGray, dlib::cv_image<unsigned char>(image));
				
				images.push_back(dlibImageGray);
				
				
				
				
				
//				ground_truth_shapes.push_back(ground_truth_shape);
//				bounding_boxs.push_back(boundingbox);

				break;
			}
		}
	}
	fin.close();
}



void TrainModel(std::vector<std::string> trainDataName,std::string _folderPath){
	
	extern string folderPath;
	folderPath = _folderPath;
	string cascadeName;
	cascadeName = _folderPath + cascadeName;

	dlib::array<array2d<unsigned char> > images_train, images_test;
	std::vector<std::vector<full_object_detection> >  faces_train, faces_test;
	
	
	for(int i=0;i<trainDataName.size();i++){
		string path;
		//        if(trainDataName[i]=="helen"||trainDataName[i]=="lfpw")
		path = _folderPath + dataPath + trainDataName[i] + "/Path_Images.txt";
		//        else
		//            path = _folderPath + dataPath + trainDataName[i] + "/Path_Images.txt";
		
		
		// LoadData(path, images, ground_truth_shapes, bounding_boxs);
		LoadOpencvBbxData(folderPath + dataPath + trainDataName[i],path, images_train, faces_train);
	}
	
	std::string  faces_directory;
//
	load_image_dataset(images_train, faces_train, faces_directory+"/training_with_face_landmarks.xml");
//	load_image_dataset(images_test, faces_test, faces_directory+"/testing_with_face_landmarks.xml");
	
	shape_predictor_trainer trainer;
	
	trainer.set_oversampling_amount(300);
	
	trainer.set_nu(0.05);
	trainer.set_tree_depth(2);
	
	
	// Tell the trainer to print status messages to the console so we can
	// see how long the training will take.
	trainer.be_verbose();
	
	// Now finally generate the shape model
	shape_predictor sp = trainer.train(images_train, faces_train);
	
	
	cout << "mean training error: "<<
	test_shape_predictor(sp, images_train, faces_train, get_interocular_distances(faces_train)) << endl;
	
	
	cout << "mean testing error:  "<<
	test_shape_predictor(sp, images_test, faces_test, get_interocular_distances(faces_test)) << endl;
	
	// Finally, we save the model to disk so we can use it later.
	serialize("sp.dat") << sp;

}
