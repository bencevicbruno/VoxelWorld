#include "renderer/models/BlockMesh.h"

BlockMesh::BlockMesh() :
    BlockMesh(UVPosition::FromAtlas(15, 15), Box(-0.5, 0.5))
{}

BlockMesh::BlockMesh(
    const UVPosition& texturePosition,
    const Box& box) :
    BlockMesh(texturePosition, texturePosition, texturePosition, texturePosition, texturePosition, texturePosition, box)
{}

BlockMesh::BlockMesh(
    const UVPosition& verticalTexturePosition,
    const UVPosition& sideTexturePosition,
    const Box& box) :
    BlockMesh(verticalTexturePosition, verticalTexturePosition, sideTexturePosition, sideTexturePosition, sideTexturePosition, sideTexturePosition, box)
{}

BlockMesh::BlockMesh(
    const UVPosition& topTexturePosition,
    const UVPosition& bottomTexturePosition,
    const UVPosition& sideTexturePosition,
    const Box& box) :
    BlockMesh(topTexturePosition, bottomTexturePosition, sideTexturePosition, sideTexturePosition, sideTexturePosition, sideTexturePosition, box)
{}

BlockMesh::BlockMesh(
    const UVPosition& topTexturePosition,
    const UVPosition& bottomTexturePosition,
    const UVPosition& northTexturePosition,
    const UVPosition& southTexturePosition,
    const UVPosition& westTexturePosition,
    const UVPosition& eastTexturePosition,
    const Box& box) :
    topTexturePosition(topTexturePosition),
    bottomTexturePosition(bottomTexturePosition),
    northTexturePosition(northTexturePosition),
    southTexturePosition(southTexturePosition),
    westTexturePosition(westTexturePosition),
    eastTexturePosition(eastTexturePosition),
    box(box)
{}

Mesh BlockMesh::getBottomFace(Vector position, Color color) const
{
    return Mesh({
        { position.x + box.minX, position.y + box.minY, position.z + box.minZ,   bottomTexturePosition.minU, bottomTexturePosition.minV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x + box.maxX, position.y + box.minY, position.z + box.minZ,   bottomTexturePosition.maxU, bottomTexturePosition.minV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x + box.maxX, position.y + box.minY, position.z + box.maxZ,   bottomTexturePosition.maxU, bottomTexturePosition.maxV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x + box.minX, position.y + box.minY, position.z + box.maxZ,   bottomTexturePosition.minU, bottomTexturePosition.maxV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a }
        }, {
            0, 1, 2, 0, 2, 3
        });
}


Mesh BlockMesh::getTopFace(Vector position, Color color) const
{
    return Mesh({
         { position.x + box.minX, position.y + box.maxY, position.z + box.minZ,   topTexturePosition.minU, topTexturePosition.minV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
         { position.x + box.maxX, position.y + box.maxY, position.z + box.minZ,   topTexturePosition.maxU, topTexturePosition.minV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
         { position.x + box.maxX, position.y + box.maxY, position.z + box.maxZ,   topTexturePosition.maxU, topTexturePosition.maxV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
         { position.x + box.minX, position.y + box.maxY, position.z + box.maxZ,   topTexturePosition.minU, topTexturePosition.maxV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a }
        }, {
            0, 2, 1, 0, 3, 2
        });
}

Mesh BlockMesh::getNorthFace(Vector position, Color color) const
{
    return Mesh({
        { position.x + box.minX, position.y + box.minY, position.z + box.minZ,   northTexturePosition.maxU, northTexturePosition.maxV, 1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x + box.maxX, position.y + box.minY, position.z + box.minZ,   northTexturePosition.minU, northTexturePosition.maxV,  1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x + box.maxX, position.y + box.maxY, position.z + box.minZ,   northTexturePosition.minU, northTexturePosition.minV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x + box.minX, position.y + box.maxY, position.z + box.minZ,   northTexturePosition.maxU, northTexturePosition.minV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a }
        }, {
            0, 2, 1, 0, 3, 2
        });
}

Mesh BlockMesh::getSouthFace(Vector position, Color color) const
{
    return Mesh({
        { position.x + box.minX, position.y + box.minY, position.z + box.maxX,  southTexturePosition.maxU, northTexturePosition.maxV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x + box.maxX, position.y + box.minY, position.z + box.maxX,  southTexturePosition.minU, northTexturePosition.maxV,    1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x + box.maxX, position.y + box.maxY, position.z + box.maxX,  southTexturePosition.minU, northTexturePosition.minV,    1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x + box.minX, position.y + box.maxY, position.z + box.maxX,  southTexturePosition.maxU, northTexturePosition.minV,    1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a }
        }, {
            0, 1, 2, 0, 2, 3
        });
}

Mesh BlockMesh::getWestFace(Vector position, Color color) const
{
    return Mesh({
        { position.x + box.minX, position.y + box.maxY, position.z + box.maxZ, westTexturePosition.minU, westTexturePosition.minV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x + box.minX, position.y + box.maxY, position.z + box.minZ, westTexturePosition.maxU, westTexturePosition.minV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x + box.minX, position.y + box.minY, position.z + box.minZ, westTexturePosition.maxU, westTexturePosition.maxV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x + box.minX, position.y + box.minY, position.z + box.maxZ, westTexturePosition.minU, westTexturePosition.maxV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a }
        }, {
            0, 1, 2, 0, 2, 3
        });
}

Mesh BlockMesh::getEastFace(Vector position, Color color) const
{
    return Mesh({
        { position.x + box.maxX, position.y + box.maxY, position.z + box.maxZ,  eastTexturePosition.minU, eastTexturePosition.minV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x + box.maxX, position.y + box.maxY, position.z + box.minZ,  eastTexturePosition.maxU, eastTexturePosition.minV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x + box.maxX, position.y + box.minY, position.z + box.minZ,  eastTexturePosition.maxU, eastTexturePosition.maxV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x + box.maxX, position.y + box.minY, position.z + box.maxZ,  eastTexturePosition.minU, eastTexturePosition.maxV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a }
        }, {
            0, 2, 1, 0, 3, 2
        });
}

Mesh BlockMesh::getCrossMesh(Vector position, Color color) const
{
    // Correction Factor = sqrt(2)/2
    constexpr float cf = 0.7071;
    Mesh mesh;

    mesh.addMesh({ {
        { position.x + box.minX * cf, position.y + box.minY, position.z + box.minZ * cf,   northTexturePosition.maxU, northTexturePosition.maxV, 1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x + box.maxX * cf, position.y + box.minY, position.z - box.minZ * cf,   northTexturePosition.minU, northTexturePosition.maxV,  1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x + box.maxX * cf, position.y + box.maxY, position.z - box.minZ * cf,   northTexturePosition.minU, northTexturePosition.minV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x + box.minX * cf, position.y + box.maxY, position.z + box.minZ * cf,   northTexturePosition.maxU, northTexturePosition.minV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a }
        }, {
            0, 2, 1, 0, 3, 2
        } });

    mesh.addMesh({ {
        { position.x + box.minX * cf, position.y + box.minY, position.z - box.maxX * cf,  southTexturePosition.maxU, northTexturePosition.maxV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x + box.maxX * cf, position.y + box.minY, position.z + box.maxX * cf,  southTexturePosition.minU, northTexturePosition.maxV,    1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x + box.maxX * cf, position.y + box.maxY, position.z + box.maxX * cf,  southTexturePosition.minU, northTexturePosition.minV,    1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x + box.minX * cf, position.y + box.maxY, position.z - box.maxX * cf,  southTexturePosition.maxU, northTexturePosition.minV,    1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a }
        }, {
            0, 1, 2, 0, 2, 3
        } });

    mesh.addMesh({ {
        { position.x + box.minX * cf, position.y + box.maxY, position.z + box.maxZ * cf, westTexturePosition.minU, westTexturePosition.minV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x - box.minX * cf, position.y + box.maxY, position.z + box.minZ * cf, westTexturePosition.maxU, westTexturePosition.minV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x - box.minX * cf, position.y + box.minY, position.z + box.minZ * cf, westTexturePosition.maxU, westTexturePosition.maxV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x + box.minX * cf, position.y + box.minY, position.z + box.maxZ * cf, westTexturePosition.minU, westTexturePosition.maxV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a }
        }, {
            0, 1, 2, 0, 2, 3
        } });

    mesh.addMesh({ {
        { position.x - box.maxX * cf, position.y + box.maxY, position.z + box.maxZ * cf,  eastTexturePosition.minU, eastTexturePosition.minV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x + box.maxX * cf, position.y + box.maxY, position.z + box.minZ * cf,  eastTexturePosition.maxU, eastTexturePosition.minV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x + box.maxX * cf, position.y + box.minY, position.z + box.minZ * cf,  eastTexturePosition.maxU, eastTexturePosition.maxV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a },
        { position.x - box.maxX * cf, position.y + box.minY, position.z + box.maxZ * cf,  eastTexturePosition.minU, eastTexturePosition.maxV,   1.0f, 1.0f, 1.0f,   color.r, color.g, color.b, color.a }
        }, {
            0, 2, 1, 0, 3, 2
        } });

    return mesh;
}




