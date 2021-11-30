#include "helpers.h"
#include "stdio.h"
#include <cs50.h>
#include <math.h>

RGBTRIPLE blur_pixel(RGBTRIPLE pixel[3][3], bool rowFirst, bool rowLast, bool colFirst, bool colLast);
RGBTRIPLE edge_pixel(RGBTRIPLE pixel[3][3], bool rowFirst, bool rowLast, bool colFirst, bool colLast);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // loop each pixel
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            // Calculate the average color weight
            int average = round((image[row][col].rgbtRed + image[row][col].rgbtGreen + image[row][col].rgbtBlue) / 3.0);
            // Apply the same weight to all colors
            image[row][col].rgbtRed = average;
            image[row][col].rgbtGreen = average;
            image[row][col].rgbtBlue = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE newImage[height][width];

    // loop each pixel to make a reflected copy
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            newImage[row][col].rgbtRed = image[row][width - 1 - col].rgbtRed;
            newImage[row][col].rgbtGreen = image[row][width - 1 - col].rgbtGreen;
            newImage[row][col].rgbtBlue = image[row][width - 1 - col].rgbtBlue;
        }
    }

    // loop each pixel to copy the reflected image to the original array
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            image[row][col] = newImage[row][col];
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE newImage[height][width];

    // loop each pixel to make a reflected copy
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            RGBTRIPLE pixelBox[3][3];

            bool rowFirst = false;
            bool rowLast = false;
            bool colFirst = false;
            bool colLast = false;

            // Check if it´s the first row and col or if it´s the last row and col.
            if (row == 0)
            {
                rowFirst = true;
            }
            if (row == (height - 1))
            {
                rowLast = true;
            }
            if (col == 0)
            {
                colFirst = true;
            }
            if (col == (width - 1))
            {
                colLast = true;
            }

            // row before (row - 1)
            // if it´s the first row jump it
            if (!rowFirst)
            {
                // col before (col - 1)
                // if it´s the first col jump it
                if (!colFirst)
                {
                    pixelBox[0][0] = image[row - 1][col - 1];
                }

                pixelBox[0][1] = image[row - 1][col];

                // col after (col + 1)
                // if it´s the last col jump it
                if (!colLast)
                {
                    pixelBox[0][2] = image[row - 1][col + 1];
                }
            }

            // current row

            // col before (col - 1)
            // if it´s the first col jump it
            if (!colFirst)
            {
                pixelBox[1][0] = image[row][col - 1];
            }

            pixelBox[1][1] = image[row][col];

            // col after (col + 1)
            // if it´s the last col jump it
            if (!colLast)
            {
                pixelBox[1][2] = image[row][col + 1];
            }


            // row after (row + 1)
            // if it´s the last row jump it
            if (!rowLast)
            {
                // col before (col - 1)
                // if it´s the first col jump it
                if (!colFirst)
                {
                    pixelBox[2][0] = image[row + 1][col - 1];
                }

                pixelBox[2][1] = image[row + 1][col];

                // col after (col + 1)
                // if it´s the last col jump it
                if (!colLast)
                {
                    pixelBox[2][2] = image[row + 1][col + 1];
                }
            }

            newImage[row][col] = blur_pixel(pixelBox, rowFirst, rowLast, colFirst, colLast);
        }
    }

    // loop each pixel to copy the reflected image to the original array
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            image[row][col] = newImage[row][col];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE newImage[height][width];

    // loop each pixel to make a reflected copy
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            RGBTRIPLE pixelBox[3][3];

            bool rowFirst = false;
            bool rowLast = false;
            bool colFirst = false;
            bool colLast = false;

            // Check if it´s the first row and col or if it´s the last row and col.
            if (row == 0)
            {
                rowFirst = true;
            }
            if (row == (height - 1))
            {
                rowLast = true;
            }
            if (col == 0)
            {
                colFirst = true;
            }
            if (col == (width - 1))
            {
                colLast = true;
            }

            // row before (row - 1)
            // if it´s the first row jump it
            if (!rowFirst)
            {
                // col before (col - 1)
                // if it´s the first col jump it
                if (!colFirst)
                {
                    pixelBox[0][0] = image[row - 1][col - 1];
                }

                pixelBox[0][1] = image[row - 1][col];

                // col after (col + 1)
                // if it´s the last col jump it
                if (!colLast)
                {
                    pixelBox[0][2] = image[row - 1][col + 1];
                }
            }

            // current row

            // col before (col - 1)
            // if it´s the first col jump it
            if (!colFirst)
            {
                pixelBox[1][0] = image[row][col - 1];
            }

            pixelBox[1][1] = image[row][col];

            // col after (col + 1)
            // if it´s the last col jump it
            if (!colLast)
            {
                pixelBox[1][2] = image[row][col + 1];
            }

            // row after (row + 1)
            // if it´s the last row jump it
            if (!rowLast)
            {
                // col before (col - 1)
                // if it´s the first col jump it
                if (!colFirst)
                {
                    pixelBox[2][0] = image[row + 1][col - 1];
                }

                pixelBox[2][1] = image[row + 1][col];

                // col after (col + 1)
                // if it´s the last col jump it
                if (!colLast)
                {
                    pixelBox[2][2] = image[row + 1][col + 1];
                }
            }

            newImage[row][col] = edge_pixel(pixelBox, rowFirst, rowLast, colFirst, colLast);
        }
    }

    // loop each pixel to copy the reflected image to the original array
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            image[row][col] = newImage[row][col];
        }
    }

    return;
}

RGBTRIPLE blur_pixel(RGBTRIPLE pixel[3][3], bool rowFirst, bool rowLast, bool colFirst, bool colLast)
{
    float divisor;
    if ((rowFirst) || (rowLast))
    {
        if ((colFirst) || (colLast))
        {
            divisor = 4.0;
        }
        else
        {
            divisor =  6.0;
        }
    }
    else
    {
        if ((colFirst) || (colLast))
        {
            divisor = 6.0;
        }
        else
        {
            divisor =  9.0;
        }
    }

    // new pixel to blur
    RGBTRIPLE newPixel;
    int iRed = 0;
    int iGreen = 0;
    int iBlue = 0;


    // Red
    if (!rowFirst)
    {
        if (!colFirst)
        {
            iRed += pixel[0][0].rgbtRed;
        }

        iRed += pixel[0][1].rgbtRed;

        if (!colLast)
        {
            iRed += pixel[0][2].rgbtRed;
        }
    }

    if (!colFirst)
    {
        iRed += pixel[1][0].rgbtRed;
    }

    iRed += pixel[1][1].rgbtRed;

    if (!colLast)
    {
        iRed += pixel[1][2].rgbtRed;
    }

    if (!rowLast)
    {
        if (!colFirst)
        {
            iRed += pixel[2][0].rgbtRed;
        }

        iRed += pixel[2][1].rgbtRed;

        if (!colLast)
        {
            iRed += pixel[2][2].rgbtRed;
        }
    }

    newPixel.rgbtRed = round(iRed / divisor);

    // Green
    if (!rowFirst)
    {
        if (!colFirst)
        {
            iGreen += pixel[0][0].rgbtGreen;
        }

        iGreen += pixel[0][1].rgbtGreen;

        if (!colLast)
        {
            iGreen += pixel[0][2].rgbtGreen;
        }
    }

    if (!colFirst)
    {
        iGreen += pixel[1][0].rgbtGreen;
    }

    iGreen += pixel[1][1].rgbtGreen;

    if (!colLast)
    {
        iGreen += pixel[1][2].rgbtGreen;
    }

    if (!rowLast)
    {
        if (!colFirst)
        {
            iGreen += pixel[2][0].rgbtGreen;
        }

        iGreen += pixel[2][1].rgbtGreen;

        if (!colLast)
        {
            iGreen += pixel[2][2].rgbtGreen;
        }
    }

    newPixel.rgbtGreen = round(iGreen / divisor);

    // Blue
    if (!rowFirst)
    {
        if (!colFirst)
        {
            iBlue += pixel[0][0].rgbtBlue;
        }

        iBlue += pixel[0][1].rgbtBlue;

        if (!colLast)
        {
            iBlue += pixel[0][2].rgbtBlue;
        }
    }

    if (!colFirst)
    {
        iBlue += pixel[1][0].rgbtBlue;
    }

    iBlue += pixel[1][1].rgbtBlue;

    if (!colLast)
    {
        iBlue += pixel[1][2].rgbtBlue;
    }

    if (!rowLast)
    {
        if (!colFirst)
        {
            iBlue += pixel[2][0].rgbtBlue;
        }

        iBlue += pixel[2][1].rgbtBlue;

        if (!colLast)
        {
            iBlue += pixel[2][2].rgbtBlue;
        }
    }

    newPixel.rgbtBlue = round(iBlue / divisor);

    return newPixel;
}

RGBTRIPLE edge_pixel(RGBTRIPLE pixel[3][3], bool rowFirst, bool rowLast, bool colFirst, bool colLast)
{
    // new pixel to edge
    RGBTRIPLE newPixel;

    if (rowFirst)
    {
        pixel[0][0].rgbtRed = 0;
        pixel[0][1].rgbtRed = 0;
        pixel[0][2].rgbtRed = 0;

        pixel[0][0].rgbtBlue = 0;
        pixel[0][1].rgbtBlue = 0;
        pixel[0][2].rgbtBlue = 0;

        pixel[0][0].rgbtGreen = 0;
        pixel[0][1].rgbtGreen = 0;
        pixel[0][2].rgbtGreen = 0;
    }

    if (rowLast)
    {
        pixel[2][0].rgbtRed = 0;
        pixel[2][1].rgbtRed = 0;
        pixel[2][2].rgbtRed = 0;

        pixel[2][0].rgbtBlue = 0;
        pixel[2][1].rgbtBlue = 0;
        pixel[2][2].rgbtBlue = 0;

        pixel[2][0].rgbtGreen = 0;
        pixel[2][1].rgbtGreen = 0;
        pixel[2][2].rgbtGreen = 0;
    }

    if (colFirst)
    {
        pixel[0][0].rgbtRed = 0;
        pixel[1][0].rgbtRed = 0;
        pixel[2][0].rgbtRed = 0;

        pixel[0][0].rgbtBlue = 0;
        pixel[1][0].rgbtBlue = 0;
        pixel[2][0].rgbtBlue = 0;

        pixel[0][0].rgbtGreen = 0;
        pixel[1][0].rgbtGreen = 0;
        pixel[2][0].rgbtGreen = 0;
    }

    if (colLast)
    {
        pixel[0][2].rgbtRed = 0;
        pixel[1][2].rgbtRed = 0;
        pixel[2][2].rgbtRed = 0;

        pixel[0][2].rgbtBlue = 0;
        pixel[1][2].rgbtBlue = 0;
        pixel[2][2].rgbtBlue = 0;

        pixel[0][2].rgbtGreen = 0;
        pixel[1][2].rgbtGreen = 0;
        pixel[2][2].rgbtGreen = 0;
    }

    // RED CALC
    int iRedX = 0;
    int iRedY = 0;
    float f;

    // Row 1
    iRedX += pixel[0][0].rgbtRed * -1;
    iRedX += pixel[0][1].rgbtRed * 0;
    iRedX += pixel[0][2].rgbtRed * 1;

    iRedY += pixel[0][0].rgbtRed * -1;
    iRedY += pixel[1][0].rgbtRed * 0;
    iRedY += pixel[2][0].rgbtRed * 1;

    // Row 2
    iRedX += pixel[1][0].rgbtRed * -2;
    iRedX += pixel[1][1].rgbtRed * 0;
    iRedX += pixel[1][2].rgbtRed * 2;

    iRedY += pixel[0][1].rgbtRed * -2;
    iRedY += pixel[1][1].rgbtRed * 0;
    iRedY += pixel[2][1].rgbtRed * 2;

    // Row 3
    iRedX += pixel[2][0].rgbtRed * -1;
    iRedX += pixel[2][1].rgbtRed * 0;
    iRedX += pixel[2][2].rgbtRed * 1;

    iRedY += pixel[0][2].rgbtRed * -1;
    iRedY += pixel[1][2].rgbtRed * 0;
    iRedY += pixel[2][2].rgbtRed * 1;

    f = sqrt(pow(iRedX, 2) + pow(iRedY, 2));
    if (f > 255)
    {
        newPixel.rgbtRed = 255;
    }
    else
    {
        newPixel.rgbtRed = round(sqrt(pow(iRedX, 2) + pow(iRedY, 2)));
    }

    // GREEN CALC
    int iGreenX = 0;
    int iGreenY = 0;
    // Row 1
    iGreenX += pixel[0][0].rgbtGreen * -1;
    iGreenX += pixel[0][1].rgbtGreen * 0;
    iGreenX += pixel[0][2].rgbtGreen * 1;

    iGreenY += pixel[0][0].rgbtGreen * -1;
    iGreenY += pixel[1][0].rgbtGreen * 0;
    iGreenY += pixel[2][0].rgbtGreen * 1;

    // Row 2
    iGreenX += pixel[1][0].rgbtGreen * -2;
    iGreenX += pixel[1][1].rgbtGreen * 0;
    iGreenX += pixel[1][2].rgbtGreen * 2;

    iGreenY += pixel[0][1].rgbtGreen * -2;
    iGreenY += pixel[1][1].rgbtGreen * 0;
    iGreenY += pixel[2][1].rgbtGreen * 2;

    // Row 3
    iGreenX += pixel[2][0].rgbtGreen * -1;
    iGreenX += pixel[2][1].rgbtGreen * 0;
    iGreenX += pixel[2][2].rgbtGreen * 1;

    iGreenY += pixel[0][2].rgbtGreen * -1;
    iGreenY += pixel[1][2].rgbtGreen * 0;
    iGreenY += pixel[2][2].rgbtGreen * 1;

    f = sqrt(pow(iGreenX, 2) + pow(iGreenY, 2));
    if (f > 255)
    {
        newPixel.rgbtGreen = 255;
    }
    else
    {
        newPixel.rgbtGreen = round(sqrt(pow(iGreenX, 2) + pow(iGreenY, 2)));
    }


    // BLUE CALC
    int iBlueX = 0;
    int iBlueY = 0;
    // Row 1
    iBlueX += pixel[0][0].rgbtBlue * -1;
    iBlueX += pixel[0][1].rgbtBlue * 0;
    iBlueX += pixel[0][2].rgbtBlue * 1;

    iBlueY += pixel[0][0].rgbtBlue * -1;
    iBlueY += pixel[1][0].rgbtBlue * 0;
    iBlueY += pixel[2][0].rgbtBlue * 1;

    // Row 2
    iBlueX += pixel[1][0].rgbtBlue * -2;
    iBlueX += pixel[1][1].rgbtBlue * 0;
    iBlueX += pixel[1][2].rgbtBlue * 2;

    iBlueY += pixel[0][1].rgbtBlue * -2;
    iBlueY += pixel[1][1].rgbtBlue * 0;
    iBlueY += pixel[2][1].rgbtBlue * 2;

    // Row 3
    iBlueX += pixel[2][0].rgbtBlue * -1;
    iBlueX += pixel[2][1].rgbtBlue * 0;
    iBlueX += pixel[2][2].rgbtBlue * 1;

    iBlueY += pixel[0][2].rgbtBlue * -1;
    iBlueY += pixel[1][2].rgbtBlue * 0;
    iBlueY += pixel[2][2].rgbtBlue * 1;

    f = sqrt(pow(iBlueX, 2) + pow(iBlueY, 2));
    if (f > 255)
    {
        newPixel.rgbtBlue = 255;
    }
    else
    {
        newPixel.rgbtBlue = round(sqrt(pow(iBlueX, 2) + pow(iBlueY, 2)));
    }

    return newPixel;
}