#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <algorithm>

extern "C" {
#include "mc920.h"
}


using namespace std;


class GeometricTransformations{
public:
	GeometricTransformations() :
        _model { 	1.0, 0, 0, 0,
					0, 1.0, 0, 0,
					0, 0, 1.0, 0,
					0, 0, 0, 1.0 }
	{};

	void print_model(){
		cout << endl;
		for (int y = 0; y < 4; ++y) {
			for (int x = 0; x < 4; ++x)
				if ( fabs(_model[y][x]) < 0.0001)
					cout << 0 << " ";
				else
					cout << _model[y][x] << " ";
			cout << endl;
		}
	};

	// _model = M * _mode
	void multiplyMatrices(double M[][4]){
		double _model_before[4][4];
		memcpy(_model_before, _model, sizeof(_model));

		for (int y = 0; y < 4; ++y) {
			for (int x = 0; x < 4; ++x) {
	     		    double val = 0;
	     			for (int z = 0; z < 4; ++z)
	    				val += _model_before[z][x] * M[y][z];
	    			_model[y][x] = val;
	    	}
	    }
	};
	
	void translate(double dx, double dy, double dz=0){
	    double translate_matrix[4][4] = {1.0, 0, 0, dx,
	                                    0, 1.0, 0, dy,
					                    0, 0, 1.0, dz,
					                    0, 0, 0, 1.0};
	    
	    multiplyMatrices(translate_matrix);
	};

	void rotate_x(double theta, bool convert_2_rad=false){
	    double _theta = theta;
		if (convert_2_rad) // parameter is degree
	    	_theta = theta * M_PI / 180.0;
	    double rotate_matrix[4][4] = {1, 0, 0,  0, 
	    							 0, cos(_theta), -sin(_theta),  0,
	                                 0, sin(_theta),  cos(_theta),  0,
					                 0, 0, 0, 1.0 };
	    
	    multiplyMatrices(rotate_matrix);
	};

	void rotate_y(double theta, bool convert_2_rad=false){
	    double _theta = theta;
	    if (convert_2_rad) // parameter is degree
	    	_theta = theta * M_PI / 180.0;
	    double rotate_matrix[4][4] = {cos(_theta), 0, sin(_theta),  0, 
	    							 0, 1.0, 0, 0,
	                                 -sin(_theta), 0, cos(_theta),  0,
					                 0, 0, 0, 1.0 };
    
	    multiplyMatrices(rotate_matrix);
	};

	void rotate_z(double theta){
	    double theta_degree = theta * M_PI / 180.0;
	    double rotate_matrix[4][4] = {cos(theta_degree), -sin(theta_degree),   0, 0,
	                                  sin(theta_degree),  cos(theta_degree),   0, 0,
					                  0, 0, 1.0, 0,
					                  0, 0,   0, 1.0 };
	    
	    multiplyMatrices(rotate_matrix);
	};
	
	void rotate_x_cos_sin(double b, double c, double d) {
		double rotate_matrix[4][4] = {1, 0, 0,  0, 
		    						  0, c/d, -b/d,  0,
		                              0, b/d,  c/d,  0,
					                  0, 0, 0, 1.0 };
		    
	    multiplyMatrices(rotate_matrix);
	}

	void rotate_y_cos_sin(double a, double d) {
	    double rotate_matrix[4][4] = {d, 0, -a,  0, 
	    							 0, 1.0, 0, 0,
	                                 a, 0, d,  0,
					                 0, 0, 0, 1.0 };
	    
	    multiplyMatrices(rotate_matrix);	
   }

	// TODO: implement scale function
	void scale(double sx, double sy, double sz);

	Point applyModelPoint(Point p, bool isVector=false) {
		double p_homo[4], p_homo_out[4];
		p_homo[0] = p.x;
		p_homo[1] = p.y;
		p_homo[2] = p.z;
		p_homo[3] = 1;

		if (isVector) {
			p_homo[3] = 0;
		}



		for (int y = 0; y < 4; ++y) {
	     	double val = 0;
			for (int x = 0; x < 4; ++x) {
   				val += this->_model[y][x] * p_homo[x];
	    	}
	    	p_homo_out[y] = val;
	    }
	    
	    Point output;
	    output.x = p_homo_out[0];
	    output.y = p_homo_out[1];
	    output.z = p_homo_out[2];

	    return output;
	};

	MedicalImage applyModelImage(MedicalImage *image) {
		MedicalImage *output = CreateMedicalImage(image->nx, image->ny, image->nz);

		for (int z = 0; z < output->nz; ++z) {
			for (int y = 0; y < output->ny; ++y) {
				for (int x = 0; x < output->nx; ++x) {
					Point p_src, p_dst;
					p_src.x = x;
					p_src.y = y;
					p_src.z = z;

					p_dst = applyModelPoint(p_src);

					cout << "(x,y,z) = " << p_src.x << ", " << p_src.y << ", " << p_src.z;
					cout << " (x',y',z') = " << p_dst.x << ", " << p_dst.y << ", " << p_dst.z << endl;

					//int  new_value = ImageValueAtPoint(image, p_dst);

				}
			}
		}
	};

private:
	double _model[4][4];

};


class Point3D {
public:
	Point3D(int x, int y, int z){
		this->point.x = x;
		this->point.y = y;
		this->point.z = z;
	};

	Point getPoint() { return point;};

	Point point;
};



class MyPlane {
public:
	MyPlane(Point3D center, Point3D normal){
		this->center = center.getPoint();
		this->normal = normal.getPoint();
	};


	void applyTransformation(GeometricTransformations T, int theta_x, int theta_y) {
		GeometricTransformations T_normal;
		T_normal.rotate_x(theta_x, true);
		T_normal.rotate_y(theta_y, true);

		this->center = T.applyModelPoint(this->center, true);
		this->normal = T_normal.applyModelPoint(this->normal, true);

		float norma = sqrt( pow(this->normal.x,2) + pow(this->normal.y,2) + pow(this->normal.z,2) );
		this->normal.x /= norma;
		this->normal.y /= norma;
		this->normal.z /= norma;

	}


	void print_plane(){
		cout << "center: " << center.x << " " << center.y << " " << center.z << " " <<
		        "normal: " << normal.x << " " << normal.y << " " << normal.z << endl;
	};

	bool isVisible(Point p) {
		Point q;
		q.x = p.x - this->center.x;
		q.y = p.y - this->center.y;
		q.z = p.z - this->center.z;


		float scalar_prod = q.x * this->normal.x +
							q.y * this->normal.y +
						    q.z * this->normal.z;
		//cout << "prod scalar " << scalar_prod << endl;
		return fabs(scalar_prod) > 0.0001;

	};

	Point center;
	Point normal;
	bool visible = false;
};


class Volume {
public:
	Volume(int width, int hight, int length){
           /*
     (0,0,0) _     5
            |   ________
               |\       \
               | \   2   \
               |0 \_______\
               |  |       |
               \  |   4   | <- 1
                \ |       |
                 \|_______|
                      3

                  |_width_|

		*/

	planes.push_back(MyPlane(Point3D(0, hight/2, length/2), Point3D (1,0,0)));
	planes.push_back(MyPlane(Point3D(width, hight/2, length/2), Point3D (-1,0,0)));

	planes.push_back(MyPlane(Point3D(width/2, hight, length/2), Point3D(0,-1,0)));
	planes.push_back(MyPlane(Point3D(width/2, 0, length/2), Point3D(0,1,0)));

	planes.push_back(MyPlane(Point3D(width/2, hight/2, length), Point3D(0,0,-1)));
	planes.push_back(MyPlane(Point3D(width/2, hight/2, 0), Point3D(0,0,1)));

	};


	void print_volume() {
		for (int i=0; i< planes.size(); ++i) {
			cout << "Plane " << i << endl;
			planes[i].print_plane();
		}

	};

	void applyTransformation(GeometricTransformations T, int theta_x, int theta_y) {
		for (int i=0; i< planes.size(); ++i) {
			planes[i].applyTransformation(T,theta_x, theta_y);
		}
	};

	bool isVisible(Point p) {
		int count = 0;
		for (int i=0; i< planes.size(); ++i) {
			if (planes[i].isVisible(p))
				++count;
		}
		return count >= 2;
	};

	std::vector<MyPlane> planes;

};



std::vector<int> DDA(Point p1, Point p2, MedicalImage *image) {
	int n;
	Point D;
	Point d;

	if (p1.x == p2.x && p1.y == p2.y && p1.z == p2.z) {
		n =1;
	}
	else {
		D.x = p2.x - p1.x;
		D.y = p2.y - p1.y;
		D.z = p2.z - p1.z;

		if ( fabs(D.x) >= fabs(D.y) && fabs(D.x) >= fabs(D.z)) {
			n = D.x + 1;
			d.x = SIGN(D.x);
			d.y = d.x * D.y / D.x;
			d.z = d.x * D.z / D.x;
		}
		else {
			if ( fabs(D.y) >= fabs(D.x) && fabs(D.y) >= fabs(D.z)) {
				n = D.y + 1;
				d.y = SIGN(D.y);
				d.x = d.y * D.x / D.y;
				d.z = d.y * D.z / D.y;
			}
			else {
				n = D.z + 1;
				d.z = SIGN(D.z);
				d.x = d.z * D.x / D.z;
				d.y = d.z * D.y / D.z;

			}
		}
	}

	Point p = p1;
	std::vector<int> Ip;
	//cout << "n safadao : " << n << endl;
	for (int i = 0; i < fabs(n); ++i) {
		if (p.x >= 0 && p.x < image->nx &&
	    	p.y >= 0 && p.y < image->ny &&
	    	p.z >= 0 && p.z < image->nz) {
			
			int image_val = ImageValueAtPoint(image, p);
			//cout <<  "image val: " << image_val << endl;
			Ip.push_back(image_val);
		}
		p.x += d.x;
		p.y += d.y;
		p.z += d.z;
	}

	return Ip;
};


int maxValue(std::vector<int> set) {
	int max = -100000;
	for(int i = 0; i < set.size(); ++i) {
		if( set[i] > max) {
			max = set[i];
		}
	}

	return max;
}

GrayImage* maxIntensityProjection(MedicalImage *image, int theta_x, int theta_y){
	GeometricTransformations T;
	GeometricTransformations T_inv;
	Volume volume(image->nx, image->ny, image->nz);

	//GrayImage *output = CreateGrayImage(500, 500);

	GrayImage *output = CreateGrayImage(image->nx, image->ny);
	Point scene_center;
	scene_center.x = image->nx / 2;
	scene_center.y = image->ny / 2;
	scene_center.z = image->nz / 2;

	// Create transformation T
	T.translate(-scene_center.x, -scene_center.y, -scene_center.z);
	T.rotate_x(theta_x, true);
	T.rotate_y(theta_y, true);
	// TODO: Compute the diagonal and use it in the last translation
	T.translate(scene_center.x, scene_center.y, scene_center.z);

	// Create transformation T_inv
	T_inv.translate(-scene_center.x, -scene_center.y, -scene_center.z);
	T_inv.rotate_x(-theta_x, true);
	T_inv.rotate_y(-theta_y, true);
	// TODO: Compute the diagonal and use it in the last translation
	T_inv.translate(scene_center.x, scene_center.y, scene_center.z);


	//volume.applyTransformation(T, theta_x, theta_y);

	for (int y = 0; y < output->ny; ++y) {
		for (int x = 0; x < output->nx; ++x) {
			std::vector<Point> candidates;
			for (int z = 0; z < image->nz; ++z) {
				Point q;
				q.x = x;
				q.y = y;
				q.z = z;

				Point q_line = T_inv.applyModelPoint(q);
				if (volume.isVisible(q)) {
					candidates.push_back(q_line);
				}
			}
			
			if (candidates.size() > 0) {
				//cout <<  "size candidates " << candidates.size() << endl;
				std::vector<int> Ipi = DDA(candidates[0], candidates.back(), image);
				//cout << "size Ipi " << Ipi.size() << endl;
				int value = int(maxValue(Ipi));
				if (value < 0) {
					//cout << "era negativo : " <<  std::to_string(value) << endl;
					value = 0;
					//return NULL;
				}

				output->val[y][x] = value;
			}
			else {
				output->val[y][x] = int(0);
			}
		}
	}


	//cout << "antes da transformacao" << endl;
	//volume.print_volume();

	volume.applyTransformation(T, theta_x, theta_y);

	//cout << "depois da transformacao" << endl;
	//volume.print_volume();

	//volume.applyTransformation(T_inv, -theta_x, -theta_y);

	//cout << " inversa" << endl;
	//volume.print_volume();

	return output;


}



// extract a planar cut from a medical image
GrayImage* getPlanarImage(Point p1, Point p2, MedicalImage *image, int dx2=-1, int dy2=-1) {
	GrayImage* output;
	if (dx2 == -1 && dy2 ==-1)
		output = CreateGrayImage(int(1 * image->nx), int(1 * image->ny));
	else
		output = CreateGrayImage(dx2, dy2);


	GeometricTransformations model_view;

	/**** vector v **************/
	Point v;

	v.x = p2.x - p1.x;
	v.y = p2.y - p1.y;
	v.z = p2.z - p1.z;

/*	cout << "---- vector v ---------" << endl << v.x << " " << v.y << " " << v.z << endl;
	cout << "p1 de v : " << p1.x << " " <<  p1.y << " " <<  p1.z << " " << endl; 
	cout << "p2 de v: " << p2.x << " " <<  p2.y << " " <<  p2.z << " " << endl; 
*/
	float norma_v = sqrt(pow(v.x,2) + pow(v.y,2) + pow(v.z,2));
	Point v_normalized = v;
	v_normalized.x /= norma_v;
	v_normalized.y /= norma_v;
	v_normalized.z /= norma_v;

/*	cout << " Normalized V " << endl;
	cout << v_normalized.x << " " << v_normalized.y << " " << v_normalized.z << endl;
*/
	/*****  ordem natural ***
	model_view.translate(-p1.x, -p1.y, -p1.z);
	double d = sqrt(pow(u_v.y,2) + pow(u_v.z,2));
	model_view.rotate_x_cos_sin(u_v.y, u_v.z, d);
	model_view.rotate_y_cos_sin(u_v.x, d);
	model_view.translate(output->nx/2, output->ny/2, 0);
	**/
	

	/******** Normal Vector  *********/
	// aplicando a inversa
	Point p1_n, p2_n;
	p1_n.x = output->nx / 2;
	p1_n.y = output->ny / 2;
	p1_n.z = 0;

	p2_n.x = output->nx / 2;
	p2_n.y = output->ny / 2;
	p2_n.z = 1;

	Point n;
	n.x = p2_n.x - p1_n.x;
	n.y = p2_n.y - p1_n.y;
	n.z = p2_n.z - p1_n.z;

	float norma_n = sqrt(pow(n.x,2) + pow(n.y,2) + pow(n.z,2));
	
	Point n_normalized = n;
	n_normalized.x /= norma_n;
	n_normalized.y /= norma_n;
	n_normalized.z /= norma_n;

/*	cout << "---- Normal  -----" << endl;
	cout << "p1 de n : " << p1_n.x << " " <<  p1_n.y << " " <<  p1_n.z << " " << endl; 
	cout << "p2 de n: " << p2_n.x << " " <<  p2_n.y << " " <<  p2_n.z << " " << endl; 
	cout << "n : " << n.x << " " <<  n.y << " " <<  n.z << " " << endl; 
	cout << "n normalized" << endl << n_normalized.x << " " << n_normalized.y << " " << n_normalized.z << endl;	
*/
	double d = sqrt(pow(v_normalized.y,2) + pow(v_normalized.z,2));

	// translada o plano de visualizacao p origem
	model_view.translate(-p1_n.x, -p1_n.y, 0);
	// rotaciona em Y (aplicar a inversa (-a))
	model_view.rotate_y_cos_sin(-v_normalized.x, d);
	// rotaciona em X aplicar a inversa (-b)
	model_view.rotate_x_cos_sin(-v_normalized.y, v_normalized.z, d);
	// move o centro do plano de visu para o centro do plano de corte
	model_view.translate(p1.x, p1.y, p1.z);


	Point n_aligned = model_view.applyModelPoint(n);
	float norma_n_aligned = sqrt(pow(n_aligned.x,2) + pow(n_aligned.y,2) + pow(n_aligned.z,2));

/*	cout << "n_aligned" << endl;
	cout << n_aligned.x << " " << n_aligned.y << " " << n_aligned.z << endl;	
	cout << "n_aligned Normalized" << endl;
	cout << n_aligned.x / norma_n_aligned << " " << n_aligned.y / norma_n_aligned << " " << n_aligned.z / norma_n_aligned << endl;	
*/	
	// diagonal principal da cena
	float D = sqrt(pow(image->nx,2) + pow(image->nz,2));
	float H = sqrt(pow(image->ny,2) + pow(D,2));
	//cout << "H = " << H << endl;

		for (int y = 0; y < output->ny; ++y)
			for (int x = 0; x < output->nx; ++x) {
				Point p_src, p_dst;
				p_src.x = x;
				p_src.y = y;
				p_src.z = 0; // - int(H/2);

				p_dst = model_view.applyModelPoint(p_src);

				//cout << "(x,y,z) = " << p_src.x << ", " << p_src.y << ", " << p_src.z;
				//cout << " (x',y',z') = " << p_dst.x << ", " << p_dst.y << ", " << p_dst.z << endl;

				int new_value = 0;
				if (p_dst.x >= 0 && p_dst.x < image->nx &&
				    p_dst.y >= 0 && p_dst.y < image->ny &&
				    p_dst.z >= 0 && p_dst.z < image->nz) {

					new_value = ImageValueAtPoint(image, p_dst);
				}

				output->val[y][x] = new_value;
			}
	
	return output;
	}


void fillSliceWithGrayImage(MedicalImage *image, GrayImage * gray_image, int slice){

  for (int y = 0; y < image->ny; ++y)
	     for (int x = 0; x < image->nx; ++x)
	        image->val[slice][y][x]  = gray_image->val[y][x];
}

MedicalImage* reformatImage(Point p1, Point p2, MedicalImage *image, int nx2=-1, int ny2=-1) {
	Point line, line_normalized;
	line.x = p2.x - p1.x;
	line.y = p2.y - p1.y;
	line.z = p2.z - p1.z;

	float length_dz = sqrt(pow(line.x,2) + pow(line.y,2) + pow(line.z,2));

	line_normalized = line;
	line_normalized.x /= length_dz;
	line_normalized.y /= length_dz;
	line_normalized.z /= length_dz;

	float step_dz = sqrt(pow(line_normalized.x,2) + pow(line_normalized.y,2) + pow(line_normalized.z,2));
	cout << "step_dz = " << step_dz << endl;

	int nz2 = length_dz / step_dz; // number of slices on the z-axis

	cout << "length_dz = " << length_dz <<  " nz2 = " << nz2 << endl;

	//return NULL;
	MedicalImage *output;
	if (nx2 == -1 && ny2 ==-1)
		output = CreateMedicalImage(image->nx, image->ny, nz2);
	else
		output = CreateMedicalImage(nx2, ny2, nz2);


	for(int slice = 0; slice < nz2; ++slice) {

		GrayImage* sliceImage =  getPlanarImage(p1,p2,image);
		fillSliceWithGrayImage(output, sliceImage,slice);
		p1.x += line_normalized.x;
		p1.y += line_normalized.y;
		p1.z += line_normalized.z;

	}

	return output;

}


void MedicalImage2GrayImages(MedicalImage *image, const char *output_dir){

	for (int z = 0; z < image->nz; ++z) {
		GrayImage *slice = CreateGrayImage(image->nx, image->ny);

		for (int y = 0; y < image->ny; ++y) {
			for (int x = 0; x < image->nx; ++x) {
				slice->val[y][x] = image->val[z][y][x];
			}
		}

		WriteGrayImage(slice, (output_dir + string("slice_") + std::to_string(z) + ".ppm").c_str());
		DestroyGrayImage(&slice);
	}
}

/*																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																							int main(){
	int main(){
	string output_dir("/home/peixe/");
	string extension(".ppm");

	MedicalImage *image = CreateMedicalImage(3, 5, 7);
	int count = 1;
	for (int k = 0; k< image->nz; ++k) 
	  for (int j = 0; j< image->ny; ++j) 
	     for (int i = 0; i< image->nx; ++i) {
	        image->val[k][j][i] = count++;
	     }     



	for (int z = 0; z < image->nz; ++z) {
	  for (int y = 0; y < image->ny; ++y) { 
	     for (int x = 0; x < image->nx; ++x) {
	        cout << image->val[z][y][x] << " ";
	     }
	     cout << endl;
	  }
	  cout << endl;
	}


	GrayImage* new_slice = CreateGrayImage(image->nx, image->ny);
	cout << "gray image" << endl;
	for (int y = 0; y < new_slice->ny; ++y) { 
	 for (int x = 0; x < new_slice->nx; ++x) {
	    cout << new_slice->val[y][x] << " ";
	 }
	 cout << endl;
	}



	fillSliceWithGrayImage(image, new_slice,2);


	cout << "after" << endl;
	for (int z = 0; z < image->nz; ++z) {
	  for (int y = 0; y < image->ny; ++y) { 
	     for (int x = 0; x < image->nx; ++x) {
	        cout << image->val[z][y][x] << " ";
	     }
	     cout << endl;
	  }
	  cout << endl;
	}



	return 0;	  



	GeometricTransformations test;

	Point p1, p2;

	p1.x = image->nx / 2;
	p1.y = image->ny / 2;
	p1.z = 3;

	p2.x = p1.x;
	p2.y = p1.y;
	p2.z = p1.z + 1;

	GrayImage * planarImage = getPlanarImage(p1, p2, image);
	WriteGrayImage(planarImage,  (output_dir + string("teste_corte_planar") + extension).c_str());

}
*/