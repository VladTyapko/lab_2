#include "sorting.h"
#include <QRandomGenerator>

Sorting::Sorting(QObject* parent) : QThread(parent){
}

void Sorting::run(){
    // first reset num_changes and num_comparisons
    num_changes = 0;
    num_comparisons = 0;

    // starts sorting
    if (algorithm == "Bubble Sort"){
        sort_bubble(arr,size);
    } else if (algorithm == "Selection Sort"){
        sort_selection(arr,size);
    } else if (algorithm == "Insertion Sort"){
        sort_insertion(arr,size);
    }

    // finish sorting
    // visualize to make sure correct num_comparisons and num_changes are shown
    color_size = 0;
    color_index = new int [color_size];
    visualize(arr,size,color_index,color_size);
    
    emit done();
}

// Shuffles the array
void Sorting::shuffle(){
    delete [] arr;
    arr = new  int[size];
    for (int i=0;i<size;++i){
        arr[i] = MAX_NUMBER*(i+1)/size;
    }
    if (shuffleType == "Random"){               // Random shuffling
        for (int i=0;i<size;++i){
            int j = QRandomGenerator::global()->bounded(0,i+1);
            swap(arr,size,i,j);
        }
    } else if (shuffleType == "Reverse"){       // Reverse order
        for (int i=0;i<size/2;++i){
            swap(arr,size,i,size-1-i);
        }
    } else if (shuffleType == "Almost sorted"){ // Swap two random elements
        int i = QRandomGenerator::global()->bounded(0,size-1);
        int j;
        do {
            j = QRandomGenerator::global()->bounded(0,size);
        } while (arr[i] == arr[j]);
        swap(arr,size,i,j);
    }

    // reset num_changes and num_comparisons
    num_changes = 0;
    num_comparisons = 0;
}

// Swap two chosen elements if they are different
void Sorting::swap(int* arr,int size,int i,int j){
    if (i>=size || j>=size || arr[i]==arr[j]){
        return;         // this line is to prevent unnecessary update of num_changes
    }
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
    num_changes += 2;   // 2 elements (at index i and j) are changed
}

// Divide an array into two smaller arrays of similar sizes and merge them into one, to be used in Merge Sort


//Find the correct place and insert the element into the place, to be used in Insertion Sort
void Sorting::insert(int* arr,int size,int i){
    if (i>size-1){
        return;
    }

    int index = i;
    // finding correct relative place to insert, linear search is used here
    for (index=i;index>0;--index){
        ++num_comparisons;
        //visualize
        color_size = 1;
        color_index = new int[color_size];
        color_index[0] = index;
        visualize(arr,size,color_index,color_size);
        delete [] color_index;
        if (arr[index-1]<arr[i]){
            break;      // quit the loop once correct place is found
        }
    }
    // insert
    int temp = arr[i];
    for (int j = i;j>index;--j){
        if (arr[j] != arr[j-1]){
            ++num_changes;
        }
        arr[j] = arr[j-1];
        //visualize
        color_size = 1;
        color_index = new int[color_size];
        color_index[0] = j;
        visualize(arr,size,color_index,color_size);
        delete [] color_index;
    }
    if (arr[index] != temp){
        ++num_changes;
    }
    arr[index] = temp;
    //visualize
    color_size = 1;
    color_index = new int[color_size];
    color_index[0] = index;
    visualize(arr,size,color_index,color_size);
    delete [] color_index;
}

//create array of specific size
void Sorting::createArray(){
    arr = new int[size];
    for (int i=0;i<size;++i){
        arr[i] = MAX_NUMBER*(i+1)/size;
    }
    shuffle();
    int index[1] = {0};
    emit changed(arr,size,index,1);
}

//Bubble Sort
void Sorting::sort_bubble(int *arr, int size){
    bool changed = true;
    int maxindex = size-1;
    while (changed){        // if nothing is changed in this iteration, the sorting is finished
        changed = false;
        for (int i=0;i<maxindex;++i){
            ++num_comparisons;
            if (arr[i]>arr[i+1]){
                swap(arr,size,i,i+1);
                changed = true;
            }
            //visualize
            color_size = 2;
            color_index= new int[color_size];
            color_index[0] = i;
            color_index[1] = i+1;
            visualize(arr,size,color_index,color_size);
            delete [] color_index;
        }
        --maxindex;     // maximum element is at correct position, no need to consider it in the next iteration
    }
}

//Insertion Sort
void Sorting::sort_insertion(int* arr, int size){
    for (int i=1;i<size;++i){
        insert(arr,size,i);
    }
}

//Selection Sort
void Sorting::sort_selection(int *arr, int size){
    for (int i=0;i<size;++i){
        // find the index with minimum element
        int minindex = i;
        int minvalue = arr[i];
        for (int j=i+1;j<size;++j){
            ++num_comparisons;
            if (arr[j]<minvalue){
                minindex = j;
                minvalue = arr[j];
            }
            //visualize
            color_size = 3;
            color_index = new int[color_size];
            color_index[0] = i;
            color_index[1] = j;
            color_index[2] = minindex;
            visualize(arr,size,color_index,color_size);
            delete [] color_index;
        }
        swap(arr,size,i,minindex);
    }
}






//Visualize the current state of the array
void Sorting::visualize(int* arr,int size_arr,int* colorindex,int size_color){
    emit changed(arr,size_arr,colorindex,size_color);
    this->usleep(animDelay);
}
