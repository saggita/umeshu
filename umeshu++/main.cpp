//
//  Copyright (c) 2011 Vladimir Chalupecky
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to
//  deal in the Software without restriction, including without limitation the
//  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
//  sell copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//  IN THE SOFTWARE.

#include "BoundarySegment.h"
#include "BoundingBox.h"
#include "ExactAdaptiveKernel.h"
#include "Node.h"
#include "Edge.h"
#include "Mesh.h"
#include "Mesher.h"
#include "Postscript_ostream.h"
#include "Triangulator.h"

typedef Mesh<ExactAdaptiveKernel> mesh;
typedef Triangulator<mesh> triangulator;
typedef Mesher<mesh> mesher;

int main (int argc, const char * argv[])
{
    try {
        mesh m;
        triangulator tri;
        tri.triangulate(Polygon::letter_a(), m);
        // tri.triangulate(Polygon::island(), m);
        // tri.triangulate(Polygon::square(1.0), m);

        Postscript_stream ps1("mesh_1.eps", m.bounding_box());
        ps1 << m;

        mesher meshgen(m);
        meshgen.make_cdt();
        Postscript_stream ps2("mesh_2.eps", m.bounding_box());
        ps2 << m;

        meshgen.refine(0.0001, 30);
        Postscript_stream ps3("mesh_3.eps", m.bounding_box());
        ps3 << m;

        std::cout << "Number of faces: " << m.number_of_faces() << std::endl;
    }
    catch (boost::exception & e) {
        std::cerr << boost::diagnostic_information(e);
    }

    return 0;
}
