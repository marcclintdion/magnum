/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include "Magnum/GL/Buffer.h"
#include "Magnum/GL/Mesh.h"
#include "Magnum/Math/Color.h"
#include "Magnum/MeshTools/CombineIndexedArrays.h"
#include "Magnum/MeshTools/CompressIndices.h"
#include "Magnum/MeshTools/Duplicate.h"
#include "Magnum/MeshTools/GenerateFlatNormals.h"
#include "Magnum/MeshTools/Interleave.h"
#include "Magnum/MeshTools/RemoveDuplicates.h"
#include "Magnum/MeshTools/Transform.h"

using namespace Magnum;
using namespace Magnum::Math::Literals;

int main() {

{
/* [combineIndexedArrays] */
std::vector<UnsignedInt> vertexIndices;
std::vector<Vector3> positions;
std::vector<UnsignedInt> normalTextureIndices;
std::vector<Vector3> normals;
std::vector<Vector2> textureCoordinates;

std::vector<UnsignedInt> indices = MeshTools::combineIndexedArrays(
    std::make_pair(std::cref(vertexIndices), std::ref(positions)),
    std::make_pair(std::cref(normalTextureIndices), std::ref(normals)),
    std::make_pair(std::cref(normalTextureIndices), std::ref(textureCoordinates))
);
/* [combineIndexedArrays] */
}

{
/* [compressIndices] */
std::vector<UnsignedInt> indices;

Containers::Array<char> indexData;
MeshIndexType indexType;
UnsignedInt indexStart, indexEnd;
std::tie(indexData, indexType, indexStart, indexEnd) =
    MeshTools::compressIndices(indices);

GL::Buffer indexBuffer;
indexBuffer.setData(indexData, GL::BufferUsage::StaticDraw);

GL::Mesh mesh;
mesh.setCount(indices.size())
    .setIndexBuffer(indexBuffer, 0, indexType, indexStart, indexEnd);
/* [compressIndices] */
}

{
/* [compressIndicesAs] */
std::vector<UnsignedInt> indices;
Containers::Array<UnsignedShort> indexData =
    MeshTools::compressIndicesAs<UnsignedShort>(indices);
/* [compressIndicesAs] */
}

{
/* [generateFlatNormals] */
std::vector<UnsignedInt> vertexIndices;
std::vector<Vector3> positions;

std::vector<UnsignedInt> normalIndices;
std::vector<Vector3> normals;
std::tie(normalIndices, normals) =
    MeshTools::generateFlatNormals(vertexIndices, positions);
/* [generateFlatNormals] */

/* [generateFlatNormals-recombine] */
std::vector<UnsignedInt> indices = MeshTools::combineIndexedArrays(
    std::make_pair(std::cref(vertexIndices), std::ref(positions)),
    std::make_pair(std::cref(normalIndices), std::ref(normals)));
/* [generateFlatNormals-recombine] */
}

{
struct MyShader {
    typedef GL::Attribute<0, Vector3> Position;
    typedef GL::Attribute<0, Vector2> TextureCoordinates;
};
/* [interleave1] */
std::vector<Vector3> positions;
std::vector<Vector2> textureCoordinates;

GL::Buffer vertexBuffer;
vertexBuffer.setData(MeshTools::interleave(positions, textureCoordinates), GL::BufferUsage::StaticDraw);

GL::Mesh mesh;
mesh.setCount(positions.size())
    .addVertexBuffer(vertexBuffer, 0, MyShader::Position{}, MyShader::TextureCoordinates{});
/* [interleave1] */
}

{
/* [interleave2] */
std::vector<Vector4> positions;
std::vector<UnsignedShort> weights;
std::vector<Color3ub> vertexColors;

auto data = MeshTools::interleave(positions, weights, 2, vertexColors, 1);
/* [interleave2] */
}

{
/* [removeDuplicates1] */
std::vector<UnsignedInt> indices;
std::vector<Vector3> positions;

indices = MeshTools::duplicate(indices, MeshTools::removeDuplicates(positions));
/* [removeDuplicates1] */
}

{
/* [removeDuplicates2] */
std::vector<Vector3> positions;
std::vector<Vector2> texCoords;

std::vector<UnsignedInt> positionIndices = MeshTools::removeDuplicates(positions);
std::vector<UnsignedInt> texCoordIndices = MeshTools::removeDuplicates(texCoords);

std::vector<UnsignedInt> indices = MeshTools::combineIndexedArrays(
    std::make_pair(std::cref(positionIndices), std::ref(positions)),
    std::make_pair(std::cref(texCoordIndices), std::ref(texCoords))
);
/* [removeDuplicates2] */
}

{
/* [transformVectors] */
std::vector<Vector3> vectors;
auto transformation = Quaternion::rotation(35.0_degf, Vector3::yAxis());
MeshTools::transformVectorsInPlace(transformation, vectors);
/* [transformVectors] */
}

{
/* [transformPoints] */
std::vector<Vector3> points;
auto transformation =
    DualQuaternion::rotation(35.0_degf, Vector3::yAxis())*
    DualQuaternion::translation({0.5f, -1.0f, 3.0f});
MeshTools::transformPointsInPlace(transformation, points);
/* [transformPoints] */
}

}
