////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2014-2016, Lawrence Livermore National Security, LLC. 
// Produced at the Lawrence Livermore National Laboratory. 
// Written by the LBANN Research Team (B. Van Essen, et al.) listed in
// the CONTRIBUTORS file. <lbann-dev@llnl.gov>
//
// LLNL-CODE-697807.
// All rights reserved.
//
// This file is part of LBANN: Livermore Big Artificial Neural Network
// Toolkit. For details, see http://software.llnl.gov/LBANN or
// https://github.com/LLNL/LBANN. 
//
// Licensed under the Apache License, Version 2.0 (the "Licensee"); you
// may not use this file except in compliance with the License.  You may
// obtain a copy of the License at:
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
// implied. See the License for the specific language governing
// permissions and limitations under the license.
//
// lbann_image_utils .cpp .hpp - Image I/O utility functions
////////////////////////////////////////////////////////////////////////////////

#ifndef LBANN_IMAGE_UTILS_HPP
#define LBANN_IMAGE_UTILS_HPP

namespace lbann
{
  class image_utils
  {
  public:
    static bool loadBMP(const char* Imagefile, int& Width, int& Height, int& BPP, bool Flip, unsigned char*& Pixels);
    static bool saveBMP(const char* Imagefile, int Width, int Height, int BPP, bool Flip, unsigned char* Pixels);
    
    static bool loadPGM(const char* Imagefile, int& Width, int& Height, int& BPP, bool Flip, unsigned char*& Pixels);
    static bool savePGM(const char* Imagefile, int Width, int Height, int BPP, bool Flip, unsigned char* Pixels);
    static bool loadPNG(const char* Imagefile, int& Width, int& Height, bool Flip, unsigned char*& Pixels);
    static bool savePNG(const char* Imagefile, int Width, int Height, bool Flip, unsigned char* Pixels);

    static bool loadJPG(const char* Imagefile, int& Width, int& Height, bool Flip, unsigned char*& Pixels);
    static bool saveJPG(const char* Imagefile, int Width, int Height, bool Flip, unsigned char* Pixels);

  };
}

#endif // LBANN_IMAGE_UTILS_HPP
