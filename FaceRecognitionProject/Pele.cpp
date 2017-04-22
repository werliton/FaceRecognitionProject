/* 
 * File:   videocapture.cpp
 * Author: gb
 *
 * Created on 2 de Novembro de 2015, 18:50
 */

#include "opencv2/opencv.hpp"
using namespace cv;

void pele(Mat &frame) {
    
    Mat tmp;  //tempor�rio para fazer a convers�o para HLS
    cvtColor(frame, tmp, COLOR_BGR2HLS);  //convers�o de cores. 

    //varre todos os pixels
    for (int i=0; i<tmp.rows-1; i++) 
        for (int j=0; j<tmp.cols-1; j++) {
            //captura cada canal
            uchar H = tmp.at<Vec3b>(i,j)[0];
            uchar L = tmp.at<Vec3b>(i,j)[1];
            uchar S = tmp.at<Vec3b>(i,j)[2];
            
            //apenas para facilitar
            double LS_ratio = ((double) L) / ((double) S);

            //usa a f�rmula
            if ((S >= 50) && (LS_ratio > 0.5) && (LS_ratio < 3.0) 
                    && ((H <= 14) || (H >= 165)) ) {
                
                //deixa branco caso encontre pele
                frame.at<Vec3b>(i,j)[0] = 255;
                frame.at<Vec3b>(i,j)[1] = 255;
                frame.at<Vec3b>(i,j)[2] = 255;
            }else {
                
                //deixa preto caso n�o seja pele
                frame.at<Vec3b>(i,j)[0] = 0;
                frame.at<Vec3b>(i,j)[1] = 0;
                frame.at<Vec3b>(i,j)[2] = 0;
            }
        }
}


int main(int, char**)
{
    VideoCapture cap(0); //abre a camera padr�o. Estou num notebook!
    if(!cap.isOpened())  //checa se funcionou
        return -1;

    namedWindow("Pele",1);  //define uma janela de apresenta��o
    for(;;)  //define um la�o infinito para pegar v�rios frames
    {
        Mat frame;   
		//CvCapture *fr;
        cap >> frame; //obtem um novo frame a partir da camera
		//cap >> fr;
        pele(frame);
		//classifier(frame);
        /*exibe o frame processado. Verifique que o primeiro
        par�metro � equivalente ao nome da janela criada */
        imshow("Pele", frame);  
        
        if(waitKey(30) >= 0) break;
    }
    return 0;
}
