#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <png.h>
#define TEXTURE_LOAD_ERROR 0

//=====================================================================
//   Load BMP image as texture
//=====================================================================

GLuint loadBMP(const char* imagepath)
{

	cout << "Reading image: " << imagepath << "\n";

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	// Actual RGB data
	unsigned char * data;

	// Open the file
    FILE* file;
    fopen_s(&file, imagepath, "rb");

	if (!file)
        {
            cout << imagepath << " could not be opened.\n";
            getchar();
            return 0;
        }

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if ( fread(header, 1, 54, file) != 54 )
        {
            cout << "Not a correct BMP file - bytes < 54\n";
            return 0;
        }
    
	// A BMP files always begins with "BM"
	if ( header[0] != 'B' || header[1] != 'M' )
        {
            cout << "Not a correct BMP file - BM missing in header\n";
            return 0;
        }
    
	// Make sure this is a 24bit file
	if ( *(int*)&(header[0x1E]) != 0  )
        {
            cout << "Not a correct 24-bit BMP file\n";
            return 0;
        }
	if ( *(int*)&(header[0x1C]) != 24 )
        {
            cout << "Not a correct BMP file\n";
            return 0;
        }

	// Read the information about the image
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)
        imageSize = width*height*3; // 3 : one byte for each RGB component
	if (dataPos == 0)
        dataPos = 54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char [imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	// Everything is in memory now, the file wan be closed
	fclose (file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	
	// "Bind" the newly created texture.
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the 2D image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	// OpenGL has now copied the data. Free the buffer.
	delete [] data;

	// Filter in the texture data.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture we just created
	return textureID;
}



/*
//=====================================================================
//   Load PNG image as texture
//=====================================================================

GLuint loadPNG(const string imagepath, int &width, int &height)
    {
            //header for testing if it is a png
        png_byte header[8];
        
            //open file as binary
        FILE *fp = fopen(imagepath.c_str(), "rb");
        if (!fp)
            {
                cout << imagepath << " could not be opened.\n";
                getchar();
                return TEXTURE_LOAD_ERROR;
            }
        
            //read the header
        fread(header, 1, 8, fp);
        
            //test if png
        int is_png = !png_sig_cmp(header, 0, 8);
        if (!is_png)
            {
                fclose(fp);
                cout << "Not a correct PNG file\n";
                return TEXTURE_LOAD_ERROR;
            }
        
            //create png struct
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
                                                     NULL, NULL);
        if (!png_ptr)
            {
                fclose(fp);
                cout << "Not a correct PNG file\n";
                return (TEXTURE_LOAD_ERROR);
            }
        
            //create png info struct
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
            {
                png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
                fclose(fp);
                cout << "Not a correct PNG file\n";
                return (TEXTURE_LOAD_ERROR);
            }
        
            //create png info struct
        png_infop end_info = png_create_info_struct(png_ptr);
        if (!end_info)
            {
                png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
                fclose(fp);
                cout << "Not a correct PNG file\n";
                return (TEXTURE_LOAD_ERROR);
            }
        
            //png error stuff, not sure libpng man suggests this.
        if (setjmp(png_jmpbuf(png_ptr)))
            {
                png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
                fclose(fp);
                cout << "Not a correct PNG file\n";
                return (TEXTURE_LOAD_ERROR);
            }
        
            //init png reading
        png_init_io(png_ptr, fp);
        
            //let libpng know you already read the first 8 bytes
        png_set_sig_bytes(png_ptr, 8);
        
            // read all the info up to the image data
        png_read_info(png_ptr, info_ptr);
        
            //variables to pass to get info
        int bit_depth, color_type;
        png_uint_32 twidth, theight;
        
            // get info about png
        png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,
                     NULL, NULL, NULL);
        
            //update width and height based on png info
        width = twidth;
        height = theight;
        
            // Update the png info struct.
        png_read_update_info(png_ptr, info_ptr);
        
            // Row size in bytes.
        int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
        
            // Allocate the image_data as a big block, to be given to opengl
        png_byte *image_data = new png_byte[rowbytes * height];
        if (!image_data)
            {
                    //clean up memory and close stuff
                png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
                fclose(fp);
                cout << "Can't read PNG data\n";
                return TEXTURE_LOAD_ERROR;
            }
        
            //row_pointers is for pointing to image_data for reading the png with libpng
        png_bytep *row_pointers = new png_bytep[height];
        if (!row_pointers)
            {
                    //clean up memory and close stuff
                png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
                delete[] image_data;
                fclose(fp);
                cout << "Can't read PNG data\n";
                return TEXTURE_LOAD_ERROR;
            }
    
            // set the individual row_pointers to point at the correct offsets of image_data
        for (int i = 0; i < height; ++i)
            row_pointers[height - 1 - i] = image_data + i * rowbytes;
        
            //read the png into image_data through row_pointers
        png_read_image(png_ptr, row_pointers);
        
            //Now generate the OpenGL texture object
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, width, height, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*) image_data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        
            //clean up memory and close stuff
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        delete[] image_data;
        delete[] row_pointers;
        fclose(fp);
        
        // Return the ID of the texture we just created
        return texture;
    }







//=====================================================================
//   Load DDS image as texture
//=====================================================================

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint loadDDS(const char * imagepath)
{
	unsigned char header[124];

	FILE *fp; 
 
	// try to open the file
	fp = fopen(imagepath, "rb"); 
	if (fp == NULL)
        {
            printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath);
            getchar();
            return 0;
        }
   
	// verify the type of file
	char filecode[4]; 
	fread(filecode, 1, 4, fp); 
	if (strncmp(filecode, "DDS ", 4) != 0)
        {
            fclose(fp);
            return 0; 
        }
	
	// get the surface desc
	fread(&header, 124, 1, fp); 

	unsigned int height      = *(unsigned int*)&(header[8 ]);
	unsigned int width	     = *(unsigned int*)&(header[12]);
	unsigned int linearSize	 = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC      = *(unsigned int*)&(header[80]);

 
	unsigned char * buffer;
	unsigned int bufsize;
	// how big is it going to be including all mipmaps?
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize; 
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char)); 
	fread(buffer, 1, bufsize, fp); 
	// close the file pointer
	fclose(fp);

//	unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4; 
	unsigned int format;
	switch(fourCC) 
        {
            case FOURCC_DXT1:
                format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; 
                break; 
            case FOURCC_DXT3: 
                format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; 
                break; 
            case FOURCC_DXT5: 
                format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; 
                break; 
            default: 
                free(buffer); 
                return 0; 
        }

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);	
	
	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16; 
	unsigned int offset = 0;

	// load the mipmaps
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) 
	{ 
		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize; 
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,  
			0, size, buffer + offset); 
	 
		offset += size; 
		width  /= 2; 
		height /= 2; 

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if(width < 1) width = 1;
		if(height < 1) height = 1;

	} 

	free(buffer); 

	return textureID;

}










GLuint loadPNG2(const char * file_name, int* width, int* height)
{

    png_byte header[8];

    FILE *fp = fopen(file_name, "rb");
    if (fp == 0)
    {
        perror(file_name);
        return 0;
    }

    // read the header
    fread(header, 1, 8, fp);

    if (png_sig_cmp(header, 0, 8))
    {
        fprintf(stderr, "error: %s is not a PNG.\n", file_name);
        fclose(fp);
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        fprintf(stderr, "error: png_create_read_struct returned 0.\n");
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        return 0;
    }

    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "error from libpng\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // init png reading
    png_init_io(png_ptr, fp);

    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    // variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 temp_width, temp_height;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
        NULL, NULL, NULL);

    if (width){ *width = temp_width; }
    if (height){ *height = temp_height; }

    //printf("%s: %lux%lu %d\n", file_name, temp_width, temp_height, color_type);

    if (bit_depth != 8)
    {
        fprintf(stderr, "%s: Unsupported bit depth %d.  Must be 8.\n", file_name, bit_depth);
        return 0;
    }

    GLint format;
    switch(color_type)
    {
    case PNG_COLOR_TYPE_RGB:
        format = GL_RGB;
        break;
    case PNG_COLOR_TYPE_RGB_ALPHA:
        format = GL_RGBA;
        break;
    default:
        fprintf(stderr, "%s: Unknown libpng color type %d.\n", file_name, color_type);
        return 0;
    }

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // glTexImage2d requires rows to be 4-byte aligned
    rowbytes += 3 - ((rowbytes-1) % 4);

    // Allocate the image_data as a big block, to be given to opengl
    png_byte * image_data = (png_byte *)malloc(rowbytes * temp_height * sizeof(png_byte)+15);
    if (image_data == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG image data\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // row_pointers is for pointing to image_data for reading the png with libpng
    png_byte ** row_pointers = (png_byte **)malloc(temp_height * sizeof(png_byte *));
    if (row_pointers == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(fp);
        return 0;
    }

    // set the individual row_pointers to point at the correct offsets of image_data
    for (unsigned int i = 0; i < temp_height; i++)
    {
        row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
    }

    // read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);

    // Generate the OpenGL texture object
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, temp_width, temp_height, 0, format, GL_UNSIGNED_BYTE, image_data);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    free(row_pointers);
    fclose(fp);
    return texture;
}

*/