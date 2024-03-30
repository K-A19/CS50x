#include "helpers.h"

#include <math.h>

int abs (int x) {
    if (x < 0) {
        return x*-1;
    }
    return x;
}

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
   for (int i = 0; i < height; i++)
   {
       for (int j = 0; j < width; j++)
       {
           //Finds the average of the red, gree, and blue channels in order to change the colour of the current pixel to a shade of gray
           int average = ((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0)+0.5;
           image[i][j].rgbtBlue = average;
           image[i][j].rgbtGreen = average;
           image[i][j].rgbtRed = average;
       }
   }
   return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
   for (int i = 0; i < height; i++)
   {
       for (int j = 0, swaps = (width / 2); j < swaps; j++)
       {
           //Does a swap with the current pixel and the pixel in the place it shall be reflected to
           RGBTRIPLE holder = image[i][j];
           image[i][j] = image[i][(width - j) - 1];
           image[i][(width - j) - 1] = holder;
       }
   }
   return;
}


// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
   //Creates a copy of the image to get original pixel values from
   RGBTRIPLE copy[height][width];
   for (int i = 0; i < height; i++)
   {
       for (int j = 0; j < width; j++)
       {
           copy[i][j] = image[i][j];
       }
   }


   for (int h = 0; h < height; h++)
   {
       for (int w = 0; w < width; w++)
       {
           //Resets all variables used for blur on each pixel
           double valid = 0.0;
           double red_average = 0;
           double green_average = 0;
           double blue_average = 0;
           for (int a = h - 1; a <= h + 1; a++)
           {
               for (int b =  w - 1; b <= w + 1; b++)
               {
                   //Checks and counts the pixels that are valid and surround the pixel in question
                   if (!(a < 0 || a >= height || b < 0 || b >= width))
                   {
                       valid++;
                       blue_average += copy[a][b].rgbtBlue;
                       green_average += copy[a][b].rgbtGreen;
                       red_average += copy[a][b].rgbtRed;
                   }
               }
           }


           //Finds the average values of all pixels in a 3x3 grid of the pixel in question
           blue_average /= valid;
           green_average /= valid;
           red_average /= valid;


           //Makes sure none of the values are set higher than 255
           if (blue_average > 255)
           {
               blue_average = 255;
           }


           if (green_average > 255)
           {
               green_average = 255;
           }


           if (red_average > 255)
           {
               red_average = 255;
           }


           //Assigns the values to the pixel in question making sure they are to the nearest integer
           image[h][w].rgbtBlue = blue_average + 0.5;
           image[h][w].rgbtGreen = green_average + 0.5;
           image[h][w].rgbtRed = red_average;
       }
   }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
   //Creates a copy of the image to get original pixel values from
   RGBTRIPLE copy[height][width];
   for (int i = 0; i < height; i++)
   {
       for (int j = 0; j < width; j++)
       {
           copy[i][j] = image[i][j];
       }
   }


   for (int h = 0; h < height; h++)
   {
       for (int w = 0; w < width; w++)
       {
           //Resets all variables used for blur on each pixel
           double GxR = 0.0, GxG = 0.0, GxB = 0.0, GyR = 0.0, GyG = 0.0, GyB = 0.0;
           double red_average = 0.0, green_average = 0.0, blue_average = 0.0;
           for (int a = -1; a <= 1; a++)
           {
               for (int b = -1; b <= 1; b++)
               {
                   if (!(w + b >= width || w + b < 0 || h + a >= height || h + a < 0))
                   {
                       GxR += copy[h + a][w + b].rgbtRed * ((2 - abs(a)) * b);
                       GxG += copy[h + a][w + b].rgbtGreen * ((2 - abs(a)) * b);
                       GxB += copy[h + a][w + b].rgbtBlue * ((2 - abs(a)) * b);


                       GyR += copy[h + a][w + b].rgbtRed * ((2 - abs(b)) * a);
                       GyG += copy[h + a][w + b].rgbtGreen * ((2 - abs(b)) * a);
                       GyB += copy[h + a][w + b].rgbtBlue * ((2 - abs(b)) * a);
                   }
               }
           }


           red_average = sqrt(GxR*GxR + GyR*GyR);
           green_average = sqrt(GxG*GxG +GyG*GyG);
           blue_average = sqrt(GxB*GxB + GyB*GyB);


           //Makes sure none of the values are set higher than 255
           if (blue_average > 255)
           {
               blue_average = 255;
           }


           if (green_average > 255)
           {
               green_average = 255;
           }


           if (red_average > 255)
           {
               red_average = 255;
           }


           //Assigns the values to the pixel in question making sure they are to the nearest integer
           image[h][w].rgbtBlue = blue_average + 0.5;
           image[h][w].rgbtGreen = green_average + 0.5;
           image[h][w].rgbtRed = red_average + 0.5;
       }
   }

   return;
}

