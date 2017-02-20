/*
 * main.c
 *
 *  Created on: Feb 13, 2017
 *      Author: Max
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error_message();
void offset_data(float* data, float offset, char* filenumber);
void scale_data(float* data, float scale, char* filenumber);
float* read_data(char* filename);
void rename_data(float* data, char* rename);
void statistics(float* data, char* filenumber);
void center(float* data, char* filenumber);
void normalize(float* data, char* filenumber);



int main(int argc, char* argv[]) {
	//parse input
	char* filenumber;
	char* rename;
	float offset = 0;
	float scale = 1;
	char* argument;
	int i, statistics_f=0, center_f=0, normalize_f=0;
	for(i=1;i<argc;i++){
		argument = argv[i];
		if(*argument != '-') error_message();
		switch(*(argument+1)) {
			case 'n':  //file name
				argument = argv[++i];
				filenumber = argument;
				break;

			case 'o':  //offset
				argument = argv[++i];
				offset=atof(argument);
				break;

			case 's':  //scale
				argument = argv[++i];
				scale=atof(argument);
				break;

			case 'r':  //rename file
				argument = argv[++i];
				rename = argument;
				break;

			case 'S':
				statistics_f = 1;
				break;

			case 'C':
				center_f = 1;
				break;

			case 'N':
				normalize_f = 1;
				break;

			case 'h': //display help
			default: error_message();
		}
	}
	//parse file number argument
	char filename[] = "Raw_data_00.txt";
	if(atoi(filenumber) < 9) { //one digit file number
		*(filename+10)=*filenumber;
	}
	else if (atoi(filenumber) < 100) { //two digit file number
		filename[10] = *(filenumber+1);
		filename[9] = *(filenumber);
	}
	else if (atoi(filenumber) >= 100) { //more than two digits = error
		printf("Error. File number too big");
		exit(1);
	}

	//open and read file
	float *data = read_data(filename);

	//perform actions dictated by arguments
	if(rename != NULL) rename_data(data,rename);
	if(offset != 0) offset_data(data,offset,filenumber);
	if(scale != 1) scale_data(data,scale,filenumber);
	if(statistics_f == 1) statistics(data,filenumber);
	if(center_f == 1) center(data,filenumber);
	if(normalize_f ==1) normalize(data,filenumber);

}

float* read_data(char* filename) {
	FILE *readfile;
	readfile = fopen(filename,"r"); //opening file
	if(readfile == NULL) { //error checking
		printf("Error opening file.");
		exit(1);
	}

	int length,max;
	fscanf(readfile,"%d %d",&length,&max);
	//float datums[length];
	float* data = malloc(sizeof(float)*length);
	*(data) = length;
	*(data+1) = max;
	for(int i = 0; i < length; i++) //scanning in data
 		fscanf(readfile,"%f",(data+i+2));

	fclose(readfile); //close file
	return data;

}
void rename_data(float* data, char* rename) {
	FILE *writefile;
	writefile = fopen(rename,"w"); //open file
	if(writefile == NULL) {
		printf("Error opening write file.");
		exit(0);
	}
	fprintf(writefile,"%.0f %.0f\n", *(data), *(data+1)); //write max and length
	for(int i = 0; i < *(data); i++) //write the rest of the data
		fprintf(writefile,"%.4f\n",*(data+i+2));
	fclose(writefile);
}


void offset_data(float* data_in, float offset, char* filenumber) {
	int length = (int) *(data_in);
	float* data = malloc(sizeof(float)*(length+2));
	memcpy(data,data_in,sizeof(float)*(length+2));

	int i;
	for(i = 0; i < length; i++) //index of data is 2 higher
		*(data+i+2) += offset;

	//parse file number argument
	char filename[] = "Offset_data_00.txt";
	if(atoi(filenumber) < 9) { //one digit file number
		*(filename+13)=*filenumber;
	}
	else if (atoi(filenumber) < 100) { //two digit file number
		filename[13] = *(filenumber+1);
		filename[12] = *(filenumber);
	}

	FILE *writefile; //same as rename_data
	writefile = fopen(filename,"w"); //open file
	if(writefile == NULL) {
		printf("Error opening write file.");
		exit(0);
	}
	fprintf(writefile,"%.0f %.4f\n", *(data), offset); //write max and length
	for(int i = 0; i < *(data); i++) //write the rest of the data
		fprintf(writefile,"%.4f\n",*(data+i+2));
	fclose(writefile);
}

void scale_data(float* data_in, float scale, char* filenumber) {
	int length = (int) *(data_in);
	float* data = malloc(sizeof(float)*(length+2));
	memcpy(data,data_in,sizeof(float)*(length+2));

	int i;
	for(i = 0; i < *(data); i++) //index of data is 1 higher
		*(data+i+2) *= scale;

	//parse file number argument
	char filename[] = "Scaled_data_00.txt";
	if(atoi(filenumber) < 9) { //one digit file number
		*(filename+13)=*filenumber;
	}
	else if (atoi(filenumber) < 100) { //two digit file number
		filename[13] = *(filenumber+1);
		filename[12] = *(filenumber);
	}

	FILE *writefile; //same as rename_data
	writefile = fopen(filename,"w"); //open file
	if(writefile == NULL) {
		printf("Error opening write file.");
		exit(0);
	}
	fprintf(writefile,"%.0f %.4f\n", *(data), scale); //write max and length
	for(int i = 0; i < *(data); i++) //write the rest of the data
		fprintf(writefile,"%.4f\n",*(data+i+2));
	fclose(writefile);
}

void statistics(float* data, char* filenumber) {
	float sum=0, max = *(data+2); //find maximum and average
	int length = (int)*(data);
	for(int i = 0; i < length; i++) {
		sum+=*(data+i+2);
		if(max < *(data+i+2))
			max = *(data+i+2);
	}
	float average = sum / length;

	//parse file number argument
	char filename[] = "Statistics_data_00.txt";
	if(atoi(filenumber) < 9) { //one digit file number
		*(filename+17)=*filenumber;
	}
	else if (atoi(filenumber) < 100) { //two digit file number
		filename[17] = *(filenumber+1);
		filename[16] = *(filenumber);
	}

	FILE *writefile; //same as rename_data
	writefile = fopen(filename,"w"); //open file
	if(writefile == NULL) {
		printf("Error opening write file.");
		exit(0);
	}
	fprintf(writefile,"%.4f %.0f\n", average, max); //write max and average
	fclose(writefile);
}

void center(float* data_in, char* filenumber) {
	int length = (int) *(data_in); //used same offset code as above
	float* data = malloc(sizeof(float)*(length+2));
	memcpy(data,data_in,sizeof(float)*(length+2));

	int i;
	float sum=0, max = *(data+2); //find max and average
	for(int i = 0; i < length; i++) {
		sum+=*(data+i+2);
		if(max < *(data+i+2))
			max = *(data+i+2);
	}
	float average = sum / length;

	for(i = 0; i < length; i++) //center / offset the data
		*(data+i+2) -= average;

	//parse file number argument
	char filename[] = "Centered_data_00.txt";
	if(atoi(filenumber) < 9) { //one digit file number
		*(filename+15)=*filenumber;
	}
	else if (atoi(filenumber) < 100) { //two digit file number
		filename[15] = *(filenumber+1);
		filename[13] = *(filenumber);
	}

	FILE *writefile; //same as rename_data
	writefile = fopen(filename,"w"); //open file
	if(writefile == NULL) {
		printf("Error opening write file.");
		exit(0);
	}
	fprintf(writefile,"%.0f %.4f\n", *(data), average * -1); //write max and length
	for(int i = 0; i < *(data); i++) //write the rest of the data
		fprintf(writefile,"%.4f\n",*(data+i+2));
	fclose(writefile);
}

void normalize(float* data_in, char* filenumber) {
	int length = (int) *(data_in); //used same offset code as above
	float* data = malloc(sizeof(float)*(length+2));
	memcpy(data,data_in,sizeof(float)*(length+2));

	int i;
	float sum=0, max = *(data+2); //find max and average
	for(int i = 0; i < length; i++)
		if(max < *(data+i+2))
			max = *(data+i+2);


	for(i = 0; i < length; i++) //normalize the data
		*(data+i+2) /= max;

	//parse file number argument
	char filename[] = "Normalized_data_00.txt";
	if(atoi(filenumber) < 9) { //one digit file number
		*(filename+13)=*filenumber;
	}
	else if (atoi(filenumber) < 100) { //two digit file number
		filename[17] = *(filenumber+1);
		filename[15] = *(filenumber);
	}

	FILE *writefile; //same as rename_data
	writefile = fopen(filename,"w"); //open file
	if(writefile == NULL) {
		printf("Error opening write file.");
		exit(0);
	}
	fprintf(writefile,"%.0f %.4f\n", *(data), 1/max); //write max and length
	for(int i = 0; i < *(data); i++) //write the rest of the data
		fprintf(writefile,"%.4f\n",*(data+i+2));
	fclose(writefile);
}

void error_message() {
	printf("ERROR. Usage: \n \t-n <file number> \n\t-o <file offset> \n\t-s <scale factor> \n\t-r <rename file> \n\t-h display help\n\t-S Get statistics\n\t-C Center the signal\n\t-N Normalize the signal\n");
	exit(0);
}

