//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimRgbToHsvSource.cpp 23664 2015-12-14 14:17:27Z dburken $
#include <ossim/imaging/ossimRgbToHsvSource.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimHsvVector.h>
#include <ossim/base/ossimRgbVector.h>
#include <ossim/imaging/ossimImageDataFactory.h>

RTTI_DEF1(ossimRgbToHsvSource, "ossimRgbToHsvSource", ossimImageSourceFilter)

ossimRgbToHsvSource::ossimRgbToHsvSource()
   :ossimImageSourceFilter(),
    theBlankTile(NULL),
    theTile(NULL)
{
}


ossimRgbToHsvSource::ossimRgbToHsvSource(ossimImageSource* inputSource)
   : ossimImageSourceFilter(inputSource),
    theBlankTile(NULL),
    theTile(NULL)
{
}

ossimRgbToHsvSource::~ossimRgbToHsvSource()
{
}

ossimRefPtr<ossimImageData> ossimRgbToHsvSource::getTile(
   const  ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if(!theInputConnection)
   {
      return ossimRefPtr<ossimImageData>();  // This filter requires an input.
   }

   ossimRefPtr<ossimImageData> inputTile =
      theInputConnection->getTile(tileRect, resLevel);   
   if(!isSourceEnabled() || !inputTile.valid())
   {
      return inputTile;
   }

   if(!theTile.valid())
   {
      allocate(); // First time through...
   }
   
   if (!theTile.valid()) // throw exeption...
   {
      return inputTile;
   }
   
   if( inputTile->getDataObjectStatus() == OSSIM_NULL ||
       inputTile->getDataObjectStatus() == OSSIM_EMPTY )
   {
      theBlankTile->setImageRectangle(tileRect);
      return theBlankTile;
   }

   if((inputTile->getNumberOfBands()==3)&&
      (inputTile->getScalarType()==OSSIM_UCHAR)&&
      (inputTile->getDataObjectStatus()!=OSSIM_NULL))
   {
      // Set the origin, resize if needed of the output tile.
      theTile->setImageRectangle(tileRect);

      float* outputBands[3];
      ossim_uint8* inputBands[3];
      outputBands[0] = static_cast<float*>(theTile->getBuf(0));
      outputBands[1] = static_cast<float*>(theTile->getBuf(1));
      outputBands[2] = static_cast<float*>(theTile->getBuf(2));
      inputBands[0] = static_cast<ossim_uint8*>(inputTile->getBuf(0));
      inputBands[1] = static_cast<ossim_uint8*>(inputTile->getBuf(1));
      inputBands[2] = static_cast<ossim_uint8*>(inputTile->getBuf(2));
      
      long height = inputTile->getHeight();
      long width  = inputTile->getWidth();
      long offset = 0;
      for(long row = 0; row < height; ++row)
      {
         for(long col = 0; col < width; ++col)
         {
            ossimRgbVector rgb(inputBands[0][offset],
                               inputBands[1][offset],
                               inputBands[2][offset]);
            
            ossimHsvVector hsv(rgb);
            
            outputBands[0][offset] = hsv.getH();
            outputBands[1][offset] = hsv.getS();
            outputBands[2][offset] = hsv.getV();
            
            ++offset;
         }
      }
   }
   else // Input tile not of correct type to process...
   {
      return inputTile;
   }
   theTile->validate();
   return theTile;
}

void ossimRgbToHsvSource::initialize()
{
   // Base class will recapture "theInputConnection".
   ossimImageSourceFilter::initialize();
}

void ossimRgbToHsvSource::allocate()
{
   theBlankTile = ossimImageDataFactory::instance()->create(this, this);
   theTile      = (ossimImageData*)theBlankTile->dup();
   theTile->initialize();
}

ossimScalarType ossimRgbToHsvSource::getOutputScalarType() const
{
   return OSSIM_NORMALIZED_FLOAT;
}

ossim_uint32 ossimRgbToHsvSource::getNumberOfOutputBands()const
{
   return 3;
}

double ossimRgbToHsvSource::getNullPixelValue()const
{
   return ossim::nan();
}

double ossimRgbToHsvSource::getMinPixelValue(ossim_uint32 /* band */)const
{
   return 0.0;
}

double ossimRgbToHsvSource::getMaxPixelValue(ossim_uint32 /* band */)const
{
   return 1.0;
}

