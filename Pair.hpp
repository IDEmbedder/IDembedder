#ifndef _INTERPOLATED_DISCRETIZATION_PAIR
#define _INTERPOLATED_DISCRETIZATION_PAIR

#include <iostream>

/**
 * Pair struct is a pair of index and weight
 */
struct Pair {
    /** index of pair */
    size_t _index;
    /** weight of pair */
    double _weight;
    /**
     * Constructor
     * @param index index of pair. Default value = 0
     * @param weight weight of pair. Default value = 0
     */
    Pair(size_t index = 0, double weight = 0.0)
        : _index(index)
        , _weight(weight) 
        {}
    
    /**
     * Operator << sends a representation of pair to the standard output
     * @param os reference to ostream
     * @param pair the object designed to representation
     * @return std ostream concatenated with this pair 
     */
    friend std::ostream& operator<<(std::ostream& os, const Pair& pair) {
        os << "index = " << pair._index << " weight = " << pair._weight << "\n";
        return os;
    }
    
    /**
     * Compares current Pair to other
     * @param other Pair to compare
     * @returm true if and only if _index and _weight are equal. 
     */
    bool operator==(const Pair& other) const {
		return ((_index == other._index) && (_weight == other._weight));
    }
};

#endif


/**
IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.

This license is for all code in this distribution. 

By downloading, copying, installing or using the software you agree to this license. If you do not
agree to this license, do not download, install, copy or use the software.

LICENSE CONDITIONS

Copyright (c) 2016, Ofir Pele, Alexey Kurbatsky
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met: 
 * Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
 * The names of its contributors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
