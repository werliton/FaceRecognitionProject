/* 
 * File:   videocapture.cpp
 * Author: gb
 *
 * Created on 2 de Novembro de 2015, 18:50
 */

#include "opencv2/opencv.hpp"
using namespace cv;

void pele(Mat &frame) {
    
    Mat tmp;  //temporário para fazer a conversão para HLS
    cvtColor(frame, tmp, COLOR_BGR2HLS);  //conversão de cores. 

    //varre todos os pixels
    for (int i=0; i<tmp.rows-1; i++) 
        for (int j=0; j<tmp.cols-1; j++) {
            //captura cada canal
            uchar H = tmp.at<Vec3b>(i,j)[0];
            uchar L = tmp.at<Vec3b>(i,j)[1];
            uchar S = tmp.at<Vec3b>(i,j)[2];
            
            //apenas para facilitar
            double LS_ratio = ((double) L) / ((double) S);

            //usa a fórmula
            if ((S >= 50) && (LS_ratio > 0.5) && (LS_ratio < 3.0) 
                    && ((H <= 14) || (H >= 165)) ) {
                
                //deixa branco caso encontre pele
                frame.at<Vec3b>(i,j)[0] = 255;
                frame.at<Vec3b>(i,j)[1] = 255;
                frame.at<Vec3b>(i,j)[2] = 255;
            }else {
                
                //deixa preto caso não seja pele
                frame.at<Vec3b>(i,j)[0] = 0;
                frame.at<Vec3b>(i,j)[1] = 0;
                frame.at<Vec3b>(i,j)[2] = 0;
            }
        }
}


int main(int, char**)
{
    VideoCapture cap(0); //abre a camera padrão. Estou num notebook!
    if(!cap.isOpened())  //checa se funcionou
        return -1;

    namedWindow("Pele",1);  //define uma janela de apresentação
    for(;;)  //define um laço infinito para pegar vários frames
    {
        Mat frame;   
		//CvCapture *fr;
        cap >> frame; //obtem um novo frame a partir da camera
		//cap >> fr;
        pele(frame);
		//classifier(frame);
        /*exibe o frame processado. Verifique que o primeiro
        parâmetro é equivalente ao nome da janela criada */
        imshow("Pele", frame);  
        
        if(waitKey(30) >= 0) break;
    }
    return 0;
}
