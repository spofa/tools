//----------------------------------------------------------------------------
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: OSSIM Kakadu based nitf writer.
//
//----------------------------------------------------------------------------
// $Id: ossimKakaduNitfWriter.h 23664 2015-12-14 14:17:27Z dburken $
#ifndef ossimKakaduNitfWriter_HEADER
#define ossimKakaduNitfWriter_HEADER 1

#include <ossim/imaging/ossimNitfWriterBase.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimString.h>

#include <iosfwd>
#include <vector>

class ossimKakaduCompressor;
class ossimKeywordlist;

class ossimKakaduNitfWriter : public ossimNitfWriterBase
{
public:

   /** @brief default constructor */
   ossimKakaduNitfWriter();

   /* @brief virtual destructor */
   virtual ~ossimKakaduNitfWriter();

   /** @return "ossim_kakadu_nitf_j2k_writer" */
   virtual ossimString getShortName() const;

   /** @return "ossim kakadu nitf j2k writer" */
   virtual ossimString getLongName()  const;

   /** @return "ossimKakaduNitfReader" */
   virtual ossimString getClassName()    const;
   
   /**
    * @brief Appends this writer image types to list "imageTypeList".
    * @param imageTypeList stl::vector<ossimString> list to append to.
    */
   virtual void getImageTypeList(std::vector<ossimString>& imageTypeList)const;
   
   virtual bool isOpen()const;   
   
   virtual bool open();

   virtual void close();
   
   /**
    * saves the state of the object.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /**
    * Will set the property whose name matches the argument
    * "property->getName()".
    *
    * @param property Object containing property to set.
    */
   virtual void setProperty(ossimRefPtr<ossimProperty> property);

   /**
    * @param name Name of property to return.
    * 
    * @returns A pointer to a property object which matches "name".
    */
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;

   /**
    * Pushes this's names onto the list of property names.
    *
    * @param propertyNames array to add this's property names to.
    */
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   bool hasImageType(const ossimString& imageType) const;

   /**
    * @brief Method to write the image to a stream.
    *
    * @return true on success, false on error.
    */
   virtual bool writeStream();

   /**
    * @brief Sets the output stream to write to.
    *
    * The stream will not be closed/deleted by this object.
    *
    * @param output The stream to write to.
    */
   virtual bool setOutputStream(std::ostream& stream);

   /**
    * Returns a 3-letter extension from the image type descriptor 
    * (theOutputImageType) that can be used for image file extensions.
    *
    * @param imageType string representing image type.
    *
    * @return the 3-letter string extension.
    */
   virtual ossimString getExtension() const;

   /**
    * Examples of writers that always generate internal
    * overviews are ossim_kakadu_jp2 and ossim_kakadu_nitf_j2k.
    *
    * @return true if the output of the writer will have
    * internal overviews. The default is false. 
    */
   virtual bool getOutputHasInternalOverviews( void ) const;

protected:
   /**
    * @brief Writes the file to disk or a stream.
    * @return true on success, false on error.
    */
   virtual bool writeFile();   

private:

   ossimKakaduCompressor* m_compressor;
   std::ostream*          m_outputStream;
   bool                   m_ownsStreamFlag;

   TYPE_DATA
};

#endif /* #ifndef ossimKakaduNitfVoid Writer_HEADER */

