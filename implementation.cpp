#include "implementation.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <random>
#include <chrono>
using namespace std;
using Clock = std::chrono::high_resolution_clock;
using Sec = std::chrono::seconds;
using Ms = std::chrono::milliseconds;
int alph_size = 256;

int* last_occ(char* pattern,int pattern_size){

	int* last_occ_table = new int[alph_size];

	for(int i = 0;i <= alph_size;i++){
		last_occ_table[i] = pattern_size;
	}
	for(int k = 0; k <= pattern_size - 2;k++){
		last_occ_table[(int)pattern[k]] = pattern_size -1 - k;
	}
	return last_occ_table;

}

int* suffix(char* pattern,int pattern_size){

	int g = pattern_size - 1;
	int f = g;
	int* suff = new int[pattern_size];
	suff[pattern_size-1] = pattern_size;

	for(int i = (pattern_size - 2); i >= 0;i--){

		if(i > g and suff[i + pattern_size - 1 - f] != i - g){
			suff[i] = min(suff[i + pattern_size - 1 - f], i - g);
		}else{
			g = min(g,i);
			f = i;
			while(g >= 0 and pattern[g] == pattern[g + pattern_size - 1 - f]){
				g--;
			}
			suff[i] = f - g;
		}
	}
	return suff;
}

int* good_suffix(char* pattern,int pattern_size){
	int* good_suff = new int[pattern_size];
	int* suff = suffix(pattern, pattern_size);
	int j = 0;

	for(int i = pattern_size - 2; i >= -1; i--){
		if(i == -1 or suff[i] == i+1){
			while(j < pattern_size-1-i){
				good_suff[j] = pattern_size - 1 - i;
				j++;
			}
		}
	}
	for(int i = 0; i <= pattern_size-2; i++){
		good_suff[pattern_size - 1 - suff[i]] = pattern_size - 1 - i;
	}
	free(suff);
	return good_suff;
}

int* border(char* pattern, int pattern_size){

	int* border_table = new int[pattern_size-1];
	border_table[-1] = -1;

	for(int t = 0; t < pattern_size-1; t++){
		border_table[t] = 0;
	}

	int i = 1;
	int j = 0;

	while((i+j) < pattern_size){
		if(pattern[i+j] == pattern[j]){
			border_table[i+j] = j+1;
			j++;
		} else {
			i = i + (j - border_table[j-1]);
			j = max(0,border_table[j-1]);
		} 
	}
	return border_table;
}

int Naive_Search(char* pattern,int pattern_size,char* input,int input_size){

	int counter = 0; // number of occurrences

	for(int j=0;j <= input_size - pattern_size;j++){
		

		// output_if function
		int i = 0;
		while(i < pattern_size and input[i + j] == pattern[i]){
			i++;
		}


		if(i == pattern_size){
			counter++;
		}

	}
	return counter;
}

int Naive_Search_lastocc(char* pattern,int pattern_size,char* input,int input_size){

	int counter = 0; // number of occurrences

	int* last_occ_table = last_occ(pattern,pattern_size);

	int j = 0;

	while((input_size - j) >= pattern_size){
		

		// output_if function
		int i = pattern_size - 1;
		while(i >= 0 and input[i + j] == pattern[i]){
			if(i == 0){
				counter++;
			}
			
			i--;
		}
		// i had a problem with the Horspool part in this algorithm,
		// since it sometimes returned a negative value,
		// to fix this i used the below max function,
		// this fixed the problem while without the alg doing something wrong
		j = j + max(last_occ_table[(int)input[j+pattern_size-1]],1);
	}
	return counter;
}

int BM(char* pattern,int pattern_size,char* input,int input_size){

	int counter = 0; // number of occurrences

	int* good_suff = good_suffix(pattern,pattern_size);
	int j = pattern_size - 1;

	while(j < input_size){
		int i = pattern_size - 1;
		while(i >= 0 and pattern[i] == input[j - pattern_size + 1 + i]){
			i--;
		}
		// output_if
		if(i < 0){
			counter++;
		}
		if(i < 0){
			j = j + good_suff[0];
		} else {
			j = j + good_suff[i];
		}

	}
	return counter;

}

int BM_last_occ(char* pattern,int pattern_size,char* input,int input_size){

	int counter = 0; // number of occurrences

	int* last_occ_table = last_occ(pattern,pattern_size);

	int* good_suff = good_suffix(pattern,pattern_size);
	int j = pattern_size - 1;

	while(j < input_size){
		int i = pattern_size - 1;
		while(i >= 0 and pattern[i] == input[j - pattern_size + 1 + i]){
			i--;
		}
		// output_if
		if(i < 0){
			counter++;
		}
		if(i < 0){
			j = j + good_suff[0];
		} else {
			//last_occ
			j = j + max(good_suff[i],last_occ_table[(int)input[j]]);
		}

	}
	return counter;
}

int BM_galil(char* pattern,int pattern_size,char* input,int input_size){

	int counter = 0;

	int* good_suff = good_suffix(pattern,pattern_size);

	int l = 0;
	int j = pattern_size - 1;

	while(j < input_size){
		int i = pattern_size - 1;
		while(i >= l and pattern[i] == input[j - pattern_size + 1 + i]){
			i--;
		}
		//output_if
		if(i < l){
			counter++;
		}
		if(i < l){
			l = pattern_size - good_suff[0];
			j = j + good_suff[0];
		} else {
			l = 0;
			j = j + good_suff[i];
		}
	}
	return counter;
}

int BM_galil_last_occ(char* pattern,int pattern_size,char* input,int input_size){

	int counter = 0;

	int* last_occ_table = last_occ(pattern,pattern_size);

	int* good_suff = good_suffix(pattern,pattern_size);

	int l = 0;
	int j = pattern_size - 1;

	while(j < input_size){
		int i = pattern_size - 1;
		while(i >= l and pattern[i] == input[j - pattern_size + 1 + i]){
			i--;
		}
		//output_if
		if(i < l){
			counter++;
		}
		if(i < l){
			l = pattern_size - good_suff[0];
			j = j + good_suff[0];
		} else {
			l = 0;
			//last_occ
			j = j + max(good_suff[i],last_occ_table[(int)input[j]]);
		}
	}
	return counter;
}

int KMP(char* pattern,int pattern_size,char* input,int input_size){

	int counter = 0;
	int* border_table = border(pattern,pattern_size);

	int i = 0;
	int j = 0;
	while(i <= input_size - pattern_size){
		if(j < pattern_size and input[i+j] == pattern[j]){
			j++;
		} else {
			if(j == pattern_size){
				counter++;
			}
			i = i + j - border_table[j - 1];
			j = max(0, border_table[j-1]);
		}
	}
	delete [] border_table;
	return counter;
}

int random(int min, int max) //range : [min, max)
{
   static bool first = true;
   if ( first ) 
   {  
      srand(time(NULL)); //seeding for the first time only!
      first = false;
   }
   return min + rand() % (max - min);
}

int main(){

	streampos begin, end;

  	ifstream infile ("ptt5");
  	ofstream outfile1 ("output.txt", std::ios_base::app);

  	begin = infile.tellg();
  	infile.seekg (0, ios::end);
  	end = infile.tellg();
  	infile.seekg (0, ios::beg);
  	int text_size = end - begin;

  	char* memblock = new char[end];
  	char* keyword;
  	int keyword_size;

  	infile.read (memblock, end);

  	//keyword_sizes should be 4,8,12,40 and text_size/16
  	//random pick of pattern that the algs are going to try to match
	auto endTime = Clock::now();
  	
	

  	outfile1 << "Naive_Search---------------" << "\n";
  	
  	for(int j = 1; j < 6;j++){

  		double nr_of_rounds = 1;
  		auto startTime = Clock::now();
  		auto duration = endTime - startTime;
  		auto durationInMs = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

  		while(durationInMs < 1000 and nr_of_rounds < 5){
  			
	  		if(j == 1){
	  			int keyword_size = 4;
	  		}else if(j == 2){
	  			int keyword_size = 8;
	  		}else if(j == 3){
	  			int keyword_size = 12;
	  		}else if(j == 4){
	  			int keyword_size = 40;
	  		}else if(j == 5){
	  			int keyword_size = text_size/16;
	  		}

  			keyword = new char[keyword_size];
	    	int pos_for_rand_pattern = random((text_size/2), (end-keyword_size-1));
	  		for(int i = 0; i < keyword_size; i++){
	  			keyword[i] = memblock[pos_for_rand_pattern+1]; 
	  		}

  			int Naive_Search_res = 0;
  			Naive_Search_res = Naive_Search(keyword,keyword_size,memblock,text_size);
  			//cout << Naive_Search_res << "\n";

  			endTime = Clock::now();
  			duration = endTime - startTime;
  			durationInMs = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  			nr_of_rounds++;
  			free(keyword);
	  	}
	
   		outfile1 << "Duration for round " << j << ": " << durationInMs << " ms" << std::endl;
   		outfile1 << "average time per run: " << (durationInMs/nr_of_rounds) << "\n";
  	}

  	

  	endTime = Clock::now();
  	outfile1 << "\n" << "Naive_Search_lastocc_res---------------" << "\n";
  	for(int j = 1; j < 6;j++){

  		double nr_of_rounds = 1;
  		auto startTime = Clock::now();
  		auto duration = endTime - startTime;
  		auto durationInMs = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

  		while(durationInMs < 1000 and nr_of_rounds < 5){
  			
	  		if(j == 1){
	  			int keyword_size = 4;
	  		}else if(j == 2){
	  			int keyword_size = 8;
	  		}else if(j == 3){
	  			int keyword_size = 12;
	  		}else if(j == 4){
	  			int keyword_size = 40;
	  		}else if(j == 5){
	  			int keyword_size = text_size/16;
	  		}

  			keyword = new char[keyword_size];
	    	int pos_for_rand_pattern = random((text_size/2), (end-keyword_size-1));
	  		for(int i = 0; i < keyword_size; i++){
	  			keyword[i] = memblock[pos_for_rand_pattern+1]; 
	  		}

  			int Naive_Search_lastocc_res = 0;
  			Naive_Search_lastocc_res = Naive_Search_lastocc(keyword,keyword_size,memblock,text_size);

  			endTime = Clock::now();
  			duration = endTime - startTime;
  			durationInMs = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  			nr_of_rounds++;
  			free(keyword);
	  	}
	
   		outfile1 << "Duration for round " << j << ": " << durationInMs << " ms" << std::endl;
   		outfile1 << "average time per run: " << (durationInMs/nr_of_rounds) << "\n";
  	}

  	
	endTime = Clock::now();
  	outfile1 << "\n" << "BM---------------" << "\n";

  	for(int j = 1; j < 6;j++){

  		double nr_of_rounds = 1;
  		auto startTime = Clock::now();
  		auto duration = endTime - startTime;
  		auto durationInMs = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

  		while(durationInMs < 1000 and nr_of_rounds < 5){
  			
	  		if(j == 1){
	  			int keyword_size = 4;
	  		}else if(j == 2){
	  			int keyword_size = 8;
	  		}else if(j == 3){
	  			int keyword_size = 12;
	  		}else if(j == 4){
	  			int keyword_size = 40;
	  		}else if(j == 5){
	  			int keyword_size = text_size/16;
	  		}

  			keyword = new char[keyword_size];
	    	int pos_for_rand_pattern = random((text_size/2), (end-keyword_size-1));
	  		for(int i = 0; i < keyword_size; i++){
	  			keyword[i] = memblock[pos_for_rand_pattern+1]; 
	  		}

  			int BM_res = 0;
  			BM_res = BM(keyword,keyword_size,memblock,text_size);

  			endTime = Clock::now();
  			duration = endTime - startTime;
  			durationInMs = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  			nr_of_rounds++;
  			free(keyword);
	  	}
	
   		outfile1 << "Duration for round " << j << ": " << durationInMs << " ms" << std::endl;
   		outfile1 << "average time per run: " << (durationInMs/nr_of_rounds) << "\n";
  	}

	/*
	 

	endTime = Clock::now();
  	outfile1 << "\n" << "BM_last_occ---------------" << "\n";
  	for(int j = 1; j < 6;j++){
  		double nr_of_rounds = 1;
  		auto startTime = Clock::now();
  		auto duration = endTime - startTime;
  		auto durationInMs = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

  		while(durationInMs < 1000 and nr_of_rounds < 5){
  			
	  		if(j == 1){
	  			int keyword_size = 4;
	  		}else if(j == 2){
	  			int keyword_size = 8;
	  		}else if(j == 3){
	  			int keyword_size = 12;
	  		}else if(j == 4){
	  			int keyword_size = 40;
	  		}else if(j == 5){
	  			int keyword_size = text_size/16;
	  		}

  			keyword = new char[keyword_size];
	    	int pos_for_rand_pattern = random((text_size/2), (end-keyword_size-1));
	  		for(int i = 0; i < keyword_size; i++){
	  			keyword[i] = memblock[pos_for_rand_pattern+1]; 
	  		}

  			int BM_last_occ_res = 0;
  			BM_last_occ_res = BM_last_occ(keyword,keyword_size,memblock,text_size);

  			endTime = Clock::now();
  			duration = endTime - startTime;
  			durationInMs = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  			nr_of_rounds++;
  			free(keyword);
	  	}
	
   		outfile1 << "Duration for round " << j << ": " << durationInMs << " ms" << std::endl;
   		outfile1 << "average time per run: " << (durationInMs/nr_of_rounds) << "\n";

  	}
  	
	
	   	

	endTime = Clock::now();
  	outfile1 << "\n" << "BM_galil---------------" << "\n";
  	for(int j = 1; j < 6;j++){

  		double nr_of_rounds = 1;
  		auto startTime = Clock::now();
  		auto duration = endTime - startTime;
  		auto durationInMs = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

  		while(durationInMs < 1000 and nr_of_rounds < 5){
  			
	  		if(j == 1){
	  			int keyword_size = 4;
	  		}else if(j == 2){
	  			int keyword_size = 8;
	  		}else if(j == 3){
	  			int keyword_size = 12;
	  		}else if(j == 4){
	  			int keyword_size = 40;
	  		}else if(j == 5){
	  			int keyword_size = text_size/16;
	  		}

  			keyword = new char[keyword_size];
	    	int pos_for_rand_pattern = random((text_size/2), (end-keyword_size-1));
	  		for(int i = 0; i < keyword_size; i++){
	  			keyword[i] = memblock[pos_for_rand_pattern+1]; 
	  		}

  			int BM_galil_res = 0;
  			BM_galil_res = BM_galil(keyword,keyword_size,memblock,text_size);

  			endTime = Clock::now();
  			duration = endTime - startTime;
  			durationInMs = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  			nr_of_rounds++;
  			free(keyword);
	  	}
	
   		outfile1 << "Duration for round " << j << ": " << durationInMs << " ms" << std::endl;
   		outfile1 << "average time per run: " << (durationInMs/nr_of_rounds) << "\n";
 	}
	
	
	
 	

	endTime = Clock::now();
   	outfile1 << "\n" << "BM_galil_last_occ---------------" << "\n";
  	for(int j = 1; j < 6;j++){

  		double nr_of_rounds = 1;
  		auto startTime = Clock::now();
  		auto duration = endTime - startTime;
  		auto durationInMs = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

  		while(durationInMs < 1000 and nr_of_rounds < 5){
  			
	  		if(j == 1){
	  			int keyword_size = 4;
	  		}else if(j == 2){
	  			int keyword_size = 8;
	  		}else if(j == 3){
	  			int keyword_size = 12;
	  		}else if(j == 4){
	  			int keyword_size = 40;
	  		}else if(j == 5){
	  			int keyword_size = text_size/16;
	  		}

  			keyword = new char[keyword_size];
	    	int pos_for_rand_pattern = random((text_size/2), (end-keyword_size-1));
	  		for(int i = 0; i < keyword_size; i++){
	  			keyword[i] = memblock[pos_for_rand_pattern+1]; 
	  		}

  			int BM_galil_last_occ_res = 0;
  			BM_galil_last_occ_res = BM_galil_last_occ(keyword,keyword_size,memblock,text_size);

  			endTime = Clock::now();
  			duration = endTime - startTime;
  			durationInMs = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  			nr_of_rounds++;
  			free(keyword);
	  	}
	
   		outfile1 << "Duration for round " << j << ": " << durationInMs << " ms" << std::endl;
   		outfile1 << "average time per run: " << (durationInMs/nr_of_rounds) << "\n";
 	}	

 	
 	
	endTime = Clock::now();
   	outfile1 << "\n" << "KMP---------------" << "\n";
  	for(int j = 1; j < 6;j++){

  		double nr_of_rounds = 1;
  		auto startTime = Clock::now();
  		auto duration = endTime - startTime;
  		auto durationInMs = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

  		while(durationInMs < 1000 and nr_of_rounds < 5){
  			
	  		if(j == 1){
	  			int keyword_size = 4;
	  		}else if(j == 2){
	  			int keyword_size = 8;
	  		}else if(j == 3){
	  			int keyword_size = 12;
	  		}else if(j == 4){
	  			int keyword_size = 40;
	  		}else if(j == 5){
	  			int keyword_size = text_size/16;
	  		}

  			keyword = new char[keyword_size];
	    	int pos_for_rand_pattern = random((text_size/2), (end-keyword_size-1));
	  		for(int i = 0; i < keyword_size; i++){
	  			keyword[i] = memblock[pos_for_rand_pattern+1]; 
	  		}

  			int KMP_res = 0;
  			KMP_res = KMP(keyword,keyword_size,memblock,text_size);

  			endTime = Clock::now();
  			duration = endTime - startTime;
  			durationInMs = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  			nr_of_rounds++;
  			delete [] keyword;
	  	}
	
   		outfile1 << "Duration for round " << j << ": " << durationInMs << " ms" << std::endl;
   		outfile1 << "average time per run: " << (durationInMs/nr_of_rounds) << "\n";
 	}	

  	*/
   	
  	outfile1.close();
  	infile.close();
}