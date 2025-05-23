#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../Helpers/fuzzyPoint.h"

#ifndef fuzzyMEANS
#define fuzzyMEANS 2
#define FUZZINESS 4
// do not go above 
// 设置样本点数量
#define NUMPOINTS (1 << 20)
// Note that this is one
// 迭代次数
#define ITERATIONS 10

/**
 * Calculate the updated version of centroids based on the data points and their association
 * @param centroids the centroids that will have their values updated based on data associated with them
 * @param data the fuzzyPoints that will be used to update the centroids based on their values and their association to that centroid.
 */
void calculateCentroids(FuzzyPoint *centroids, FuzzyPoint *data){
    for(int i = 0 ; i < NUMCLUSTER ; i++){
        for(int j = 0 ; j < DIMENSIONS; j++){
            float probSum = 0.0f;
            float pointSum = 0.0f;
            for(int k = 0 ; k < NUMPOINTS ; k++){
                probSum += powf(data[k].clusters[i],FUZZINESS);
                pointSum += data[k].values[j]  * powf(data[k].clusters[i],FUZZINESS);
            }
            centroids[i].values[j] = pointSum / probSum;
        }
    }
}

/**
 * Calculate the updated association of data point to a centroid
 * @param data the fuzzyPoints that will have their values updated based on centroids they are associated with
 * @param centroids the centroids that will be used to update the fuzzyPoints based on their values and their association to that fuzzyPoint.
 * @param centroid the centroid we are measuring association of
 */
float getNewValue(FuzzyPoint data, FuzzyPoint centroid, FuzzyPoint *centroids){
    float p = 2.0f / (FUZZINESS-1);
    float sum = 0.0f;
    float temp;
    float distDataCentroid = distance(data,centroid);
    for(int i = 0 ; i < NUMCLUSTER ; i++){
        temp = distDataCentroid / distance(data, centroids[i]);
        temp = pow(temp,p);
        sum += temp;
    }
    return 1.0f / sum;
}

/**
 * Calculate the updated association of data points based on the centroids and their association
 * @param data the fuzzyPoints that will have their values updated based on centroids they are associated with
 * @param centroids the centroids that will be used to update the fuzzyPoints based on their values and their association to that fuzzyPoint.
 */
void updateDataAssignment(FuzzyPoint *centroids, FuzzyPoint *data){
    // For every point
    for(int i = 0 ; i < NUMPOINTS; i++){
        // For point's association to that specific cluster
        for(int j = 0 ; j < NUMCLUSTER ; j++){
            float assoc = getNewValue(data[i], centroids[j], centroids);
            // printf("point: %d has %f assoc to centroid %d \n",i,assoc,j);
            data[i].clusters[j] = assoc;
        }
    }
}


/**
 * Highest level of data initialisation.
 * @param data set of FuzzyPoints that will be data that will now all be initialized
 */
void initAllFuzzyPoints(FuzzyPoint *data){
    for(int i = 0 ; i < NUMPOINTS ; i++){
        // can make a function to automate this
        initFuzzyPoint(&data[i]);
    }
}
/**
 * Highest level of printing Centroids.
 * @param centroids set of centroids that will be printed.
 */
void printCentroids(FuzzyPoint *centroids){
    printf("######################################################################################################\n");
    printf("Centroids \n");
    for(int i = 0 ; i < NUMCLUSTER ; i++){
        printFuzzyPoint(centroids[i]);
    }
    printf("######################################################################################################\n");
}
/**
 * Highest level of printing fuzzyPoints.
 * @param data set of fuzzyPoints that will be printed.
 */
void printAllFuzzyPoints(FuzzyPoint *data){
    printf("######################################################################################################\n");
    printf("Fuzzy Data Points \n");
    for(int i = 0 ; i < NUMPOINTS ; i++){
        printFuzzyPoint(data[i]);
    }
    printf("######################################################################################################\n");
}

/**
 * 读取样本点数据
 * @param fname 样本点数据文件路径
 * @param data 样本点数据
 * @param pointNum 样本点数量
 * @param dimension 样本点维度
 */
 void readData(const char * fname, FuzzyPoint *data,int pointNum,int dimension){
  FILE *csv_data = fopen(fname, "r");
  if (!csv_data) {
      printf("Error: Unable to open file for reading - %s\n", fname);
      exit(1);
  } else {
      char line[8192];
      printf("Open file successfully!\n");
      // 读取标题行
      fgets(line, sizeof(line), csv_data);
      int i = 0;
      while (fgets(line, sizeof(line), csv_data) && i < pointNum) {
          char *token = strtok(line, ",");
          int j = 0;
          initProb(&data[i]);
          while (token && j < dimension) {
              data[i].values[j] = atof(token);
              token = strtok(NULL, ",");
              j++;
          }
          i++;
      }
      fclose(csv_data);
      printf("Close file successfully!\n");
  }
}

#endif