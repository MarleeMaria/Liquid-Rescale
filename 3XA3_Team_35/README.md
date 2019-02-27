# 3XA3 Project - Team 35 - "Team Marshiel"

## Members

* Marlee Roth
* Harsh Shah
* Daniel Wolff

## Software Purpose

The purpose of a seam carving algorithm is to intelligently scale an image in order to prevent  the deformation of important features. The algorithm finds seams within the image and removes the ones that will affect the image the least, preserving a clear image containing significant features. A seam is vertical or horizontal sequence of pixels of least significance, each of which only have one pixel per row or column (depending on the orientation of the seam).

## Software Scope

The scope of the Seam carving algorithm is vast. The open sourced code allows a user to scale an image up or down, amplify objects, as well as remove objects from the image. However for our project we shall be focusing on the scaling of the images as well as the removal and preservation of objects. We shall make use of a commonly used C++ library to implement these features. We plan on implementing an object selection tool using a edge detection algorithm.

## SETUP FOR DEVELOPMENT

### Linux

``` bash
sudo apt-get update
sudo apt-get install glade libgtkmm-3.0 liblqr-1-0 glib-2.0
```