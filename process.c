/* This coursework specification, and the example code provided during the
 * course, is Copyright 2024 Heriot-Watt University.
 * Distributing this coursework specification or your solution to it outside
 * the university is academic misconduct and a violation of copyright law. */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* The RGB values of a pixel. */
struct Pixel {
    int red;
    int green;
    int blue;
};

/* An image loaded from a file. */

// The purpoose of the Image struct is to store the dimensions (width and height) and the pixel data of an image loaded from a file.
struct Image {
    /* TODO: Question 1 */
    int width;        // Width of the image
    int height;       // Height of the image
    struct Pixel *data; // Pointer to dynamically-allocated array of Pixels
    char *output_Filename; // Filename to save output to (e.g. "output.hs16")

    // the purpose of the Image struct is to hold the width, height and pixel data of an image loaded from a file.
};

// The purpose of the ImageNode struct is to store a pointer to an Image struct, and a pointer to the next ImageNode in the linked list.
struct ImageNode {
    struct Image* image; // Pointer to an Image struct
    struct ImageNode* next; // Pointer to the next ImageNode in the linked list
};


/* Free a struct Image */

// The purpose of the free_image is to free the memory that was dynamically allocated for the Image, including the pixel data array.
// This is important to avoid memory leaks.
void free_image(struct Image *img)
{
    /* TODO: Question 2a */

    // To free the image, we need to free the pixel data, then free the Image struct itself.
    if (img != NULL) 
    {
        free(img->data); // Free the dynamically allocated pixel data
        free(img); // Free the Image struct itself

    // This allows an image to be loaded from a file, then freed later.
    }
}

/* Opens and reads an image file, returning a pointer to a new struct Image.
 * On error, prints an error message and returns NULL. */
struct Image *load_image(const char *filename)
/**
 * Loads an image from a file into a newly allocated Image struct.
 * Opens the file, reads the width, height and pixel data into 
 * the Image struct. Performs error checking and cleanup on failure.
 * Returns pointer to loaded Image, or NULL on failure.
*/
{
    /* Open the file for reading */
    FILE *f = fopen(filename, "r");
    if (f == NULL) 
    {
        // If the file could not be opened, print an error message
        fprintf(stderr, "File %s could not be read.\n", filename);
        // Return NULL to indicate failure
        return NULL;
    }

    /* Allocate the Image object, and read the image from the file */
    /* TODO: Question 2b */

    /* Read the header */
    char header[6]; // Assuming the header is 5 characters followed by a null terminator
    if (fscanf(f, "%5s", header) != 1 || strcmp(header, "HSDEC") != 0) 
    {
        // If the header is not "HSDEC", print an error message
        fprintf(stderr, "Invalid header in file %s.\n", filename);
        fclose(f);
        // Return NULL to indicate failure
        return NULL;
    }

    /* Read the width and height */
    int width, height;
    if(fscanf(f, "%d %d", &width, &height)!= 2) 
    {
        // If the width and height could not be read, print an error message
        fprintf(stderr, "File %s could not be read.\n", filename);
        fclose(f);
        // Return NULL to indicate failure
        return NULL;
    }

    // Allocate memory for the Image struct
    struct Image *img = malloc(sizeof(struct Image));
    // Check if memory allocation was successful
    if (img == NULL) 
    {
        // If memory allocation fails, print an error message
        fprintf(stderr, "Memory allocation failed.\n");
        fclose(f);
        // Return NULL to indicate failure
        return NULL;
    }

    // Set the Image struct fields (width and height)
    img->width = width;
    img->height = height;

    // Allocate memory for the pixel data array in the Image struct (width * height) 
    img->data = malloc(sizeof(struct Pixel) * width * height);
    // Check if memory allocation was successful
    if (img->data == NULL) 
    {
        // If memory allocation fails, print an error message
        fprintf(stderr, "Memory allocation failed.\n");
        // Free allocated memory
        free_image(img);
        fclose(f);
        // Return NULL to indicate failure
        return NULL;
    }

    // Read the pixel data from the file
    for(int i = 0; i < width * height; i++)
    {
        int r, g, b;
        if(fscanf(f, "%d %d %d",&img->data[i].red, &img->data[i].green, &img->data[i].blue)!= 3) {
            // If the pixel data could not be read, print an error message
            fprintf(stderr, "File %s could not be read.\n", filename);
            // Free allocated memory
            free_image(img);
            fclose(f);
            // Return NULL to indicate failure
            return NULL;
        }
    }

    // Close the file
    fclose(f);
    // Return the pointer to the loaded image struct
    return img;
}

/* Write img to file filename. Return true on success, false on error. */

/* The pupose of this function is to save the image data from an Image struct to a file. 
   It writes the width and height of the image, then the pixel data to the file
   so the image can be loaded again later. */
bool save_image(const struct Image *img, const char *filename)
{
    /* TODO: Question 2c */ 
    // Check if the image is NULL
    if(img == NULL) 
    {
        // If the image is NULL, print an error message
        fprintf(stderr, "Image is NULL.\n");
        // Return false to indicate failure
        return false;
    }

    FILE *f = fopen(filename, "w"); // Open the file for writing
    if (f == NULL) 
    {
        // If the file could not be opened, print an error message
        fprintf(stderr, "File %s could not be written.\n", filename);
        // Return false to indicate failure
        return false;
    }
    fprintf(f, "HSDEC "); // Write the header

    // Write the width and height of the image
    fprintf(f, "%d %d\n", img->width, img->height);
    // Write the pixel data
    for(int i = 0; i < img->width * img->height; i++)
    {
        // Write the pixel data
        fprintf(f, "%d %d %d\n", img->data[i].red, img->data[i].green, img->data[i].blue);
    }
    // Close the file
    fclose(f);
    // Return true to indicate success
    return true;
}

/* Allocate a new struct Image and copy an existing struct Image's contents
 * into it. On error, returns NULL. */

 // The copy_image function copies an existing image into a new image.
struct Image *copy_image(const struct Image *source)
{
    /* TODO: Question 2d */
    if (source == NULL) 
    {
        // If the source is NULL, print an error message
        fprintf(stderr, "Source image is NULL.\n");
        // Return NULL to indicate failure
        return NULL;
    }
    // Allocate memory for the new Image struct
    struct Image *new_img = malloc(sizeof(struct Image));
    // Check if memory allocation was successful
    if (new_img == NULL) 
    {
        // If memory allocation fails, print an error message
        fprintf(stderr, "Memory allocation failed.\n");
        // Return NULL to indicate failure
        return NULL;
    }
    // Set the new Image struct fields (width and height)
    new_img->width = source->width;
    new_img->height = source->height;

    // Allocate memory for the pixel data array in the new Image struct (width * height)
    new_img->data = malloc(sizeof(struct Pixel) * source->width * source->height);
    // Check if memory allocation was successful
    if (new_img->data == NULL) 
    {
        // If memory allocation fails, print an error message
        fprintf(stderr, "Memory allocation failed.\n");
        // Free allocated memory
        free_image(new_img);
        // Return NULL to indicate failure
        return NULL;
    }
    // Copy the pixel data from the source Image struct to the new Image struct
    for(int i = 0; i < source->width * source->height; i++)
    {
        new_img->data[i].red = source->data[i].red;
        new_img->data[i].green = source->data[i].green;
        new_img->data[i].blue = source->data[i].blue;
    }
    // Return the pointer to the new Image struct
    return new_img;

}

/* Perform your first task. */
struct Image *apply_MONO(const struct Image *source)
{
    /* TODO: Question 3 */

    // Check if the source is NULL
    if (source == NULL) 
    {
        // If the source is NULL, print an error message
        fprintf(stderr, "Source image is NULL.\n");
        // Return NULL to indicate failure
        return NULL;
    }

    // Allocate memory for the output Image struct
    struct Image *output_image = malloc(sizeof(struct Image));

    // Check if memory allocation was successful
    if (output_image == NULL) 
    {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    // Set the output Image struct fields (width and height)
    output_image->width = source->width;
    output_image->height = source->height;


    // Allocate memory for the pixel data array in the output Image struct (width * height)
    output_image->data = malloc(sizeof(struct Pixel) * source->width * source->height);
    // Check if memory allocation was successful
    if (output_image->data == NULL) 
    {
    // If memory allocation fails, print an error message
    fprintf(stderr, "Memory allocation failed.\n");
    // Free allocated memory
    free(output_image);
    // Return NULL to indicate failure
    return NULL;
    }

    // Copy the pixel data from the source Image struct to the output Image struct
    for(int i = 0; i < source->width * source->height; i++)
    {
        float grey = 0.299 * source->data[i].red + 0.587 * source->data[i].green + 0.114 * source->data[i].blue;
        output_image->data[i].red = (int)grey;
        output_image->data[i].green = (int)grey;
        output_image->data[i].blue = (int)grey;
    }   

    // Return the pointer to the output Image struct
    return output_image;
}


/* Perform your second task. */
bool apply_NORM(struct Image *source) { // const is not needed here

    // Check if the source is NULL
    if (source == NULL) {
        fprintf(stderr, "Source image is NULL.\n");
        return false;
    }
    
    int min_val = 255;
    int max_val = 0;

    // Find the overall minimum and maximum values across all color channels
    for(int i = 0; i < source->width * source->height; i++) {
        if (source->data[i].red < min_val) min_val = source->data[i].red;
        if (source->data[i].green < min_val) min_val = source->data[i].green;
        if (source->data[i].blue < min_val) min_val = source->data[i].blue;

        if (source->data[i].red > max_val) max_val = source->data[i].red;
        if (source->data[i].green > max_val) max_val = source->data[i].green;
        if (source->data[i].blue > max_val) max_val = source->data[i].blue;
    }

    printf("Minimum value: %d\n", min_val);
    printf("Maximum value: %d\n", max_val);

    // Normalize the image
    int range = max_val - min_val;

    // Check if the image is already normalized
    if(range == 0){
        fprintf(stderr, "Image is already normalized.\n");
        return true; // Return true to indicate success
    }

    for(int i = 0; i < source->width * source->height; i++) {
        source->data[i].red = (source->data[i].red - min_val) * 255 / range;
        source->data[i].green = (source->data[i].green - min_val) * 255 / range;
        source->data[i].blue = (source->data[i].blue - min_val) * 255 / range;
    }

    // Return true to indicate success
    return true;
}


int main(int argc, char *argv[]) {

    // Ensure there's an even number of arguments besides the program name, indicating pairs of input/output files
    if (argc < 3 || argc % 2 != 1) {
        fprintf(stderr, "Usage: %s INPUTFILE1 OUTPUTFILE1 [INPUTFILE2 OUTPUTFILE2 ...]\n", argv[0]);
        return 1;
    }

    // Process each pair of input/output files
    for (int i = 1; i < argc; i += 2) {
        // Load the input image
        struct Image *in_img = load_image(argv[i]);
        if (in_img == NULL) {
            continue; // Skip to the next pair if loading fails
        }

    // Apply the first processing step (monochrome conversion) to the input image
    struct Image *mono_img = apply_MONO(in_img);
    if (mono_img == NULL) 
    {
        fprintf(stderr, "Monochrome conversion failed for %s.\n", argv[i]);
        free_image(in_img);
        continue; // Skip to the next pair
    }

        // Save the processed (monochrome) image
    if (!save_image(mono_img, argv[i + 1])) 
        {
            fprintf(stderr, "Saving image to %s failed.\n", argv[i + 1]);
        }

    // Apply the second processing step (normalization) directly to the monochrome image
     if (!apply_NORM(mono_img)) 
    {
        fprintf(stderr, "Normalization failed for %s.\n", argv[i]);
        free_image(mono_img); // Free the memory for the monochrome image
        continue; // Skip to the next pair
    }

    // Free the memory for both the input and processed images
    free_image(in_img);
    free_image(mono_img);

    }

    return 0;
}

