#include <stdio.h>
#include <stdint.h>

// Memory address of LED register
volatile unsigned int *		gDebugLedsMemoryMappedRegister = (unsigned int *)0x2000;

// Type of data to sort
#define _TYPE_ uint8_t

// Data to sort
const int N = 256;
_TYPE_ array[256] = {255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 237, 236, 235, 234, 233, 232, 231, 230, 229, 228, 227, 226, 225, 224, 223, 222, 221, 220, 219, 218, 217, 216, 215, 214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 204, 203, 202, 201, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 189, 188, 187, 186, 185, 184, 183, 182, 181, 180, 179, 178, 177, 176, 175, 174, 173, 172, 171, 170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148, 147, 146, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};


// Copy n elements from src to dest
void copy(_TYPE_* dest, _TYPE_* src, int n) {
	for (int i = 0; i < n; i++) {
		dest[i] = src[i];
	}
}

// Swap values at a and b
void swap(_TYPE_* a, _TYPE_* b) {
    int c = *a;
    *a = *b;
    *b = c;
}

// Bubblesort an array in-place
_TYPE_* bubblesort(_TYPE_* arr, int n) {
    int swaps = 1;

    while (swaps > 0) {
        swaps = 0;

        for (int i = 0; i < n-1; i++) {
            if (arr[i] > arr[i+1]) {
                swap(arr+i, arr+i+1);
                swaps++;
            }
        }
    }

    

    return arr;


}

typedef unsigned char uint8_t;

uint8_t gencrc(uint8_t *data, size_t len)
{
    uint8_t crc = 0xff;
    size_t i, j;
    for (i = 0; i < len; i++) {
        crc ^= data[i];
        for (j = 0; j < 8; j++) {
            if ((crc & 0x80) != 0)
                crc = (uint8_t)((crc << 1) ^ 0x31);
            else
                crc <<= 1;
        }
    }
    return crc;
}

	
int main(void) {

	enum
	{
		kSpinDelay = 100000,
	};

	*gDebugLedsMemoryMappedRegister = 0x00;
	_TYPE_ unsorted[256];

	uint8_t output;

	while (1) {

		*gDebugLedsMemoryMappedRegister ^= 0xFF;
		copy(unsorted, array, N);
		bubblesort(unsorted, N);
		
		output = gencrc(unsorted, 256);

		*gDebugLedsMemoryMappedRegister ^= 0xFF;
		for (int j = 0; j < kSpinDelay; j++);
		*gDebugLedsMemoryMappedRegister ^= 0xFF;
		for (int j = 0; j < kSpinDelay; j++);

		for (int k = 0; k < 8; k++) {

			if (((output >> (7-k)) & 1) == 0) {
				*gDebugLedsMemoryMappedRegister = 0x00;
			} else {
				*gDebugLedsMemoryMappedRegister = 0xFF;
			}

			for (int j = 0; j < kSpinDelay; j++);

		}

	}

}
