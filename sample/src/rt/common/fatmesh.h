/*****************************************************************************
 * Copyright (C) 2020 - 2023 OPPO. All rights reserved.
 *******************************************************************************/

// This file is part of <ph/va.h>. Do NOT include it directly from your source code. Include <ph/va.h> instead.

#include <vector>

namespace ph {

/// A universal (thus very fat) triangle mesh class. TODO: move t RG (along with Eigen)
struct FatMesh {
    std::vector<Eigen::Vector3f> position;
    std::vector<Eigen::Vector3f> normal;
    std::vector<Eigen::Vector3f> tangent;
    std::vector<Eigen::Vector2f> texcoord;
    Eigen::AlignedBox3f          bbox;

    /// Disable copy semantics
    PH_NO_COPY(FatMesh);

    /// Default constructor
    FatMesh() = default;

    /// Move constructor
    FatMesh(FatMesh && rhs) { moveFrom(rhs); }

    /// Move operator
    FatMesh & operator=(FatMesh && rhs) {
        moveFrom(rhs);
        return *this;
    }

    /// Check if the mesh is empty or not.
    bool empty() const { return position.empty(); }

    /// bool operator
    operator bool() const { return !empty(); }

    /// Load from .OBJ stream, comining all shapes into one mesh.
    static FatMesh loadObj(std::istream &);

    /// Load from .OBJ file.
    static FatMesh loadObj(const std::string & filename) {
        std::ifstream f(filename);
        if (!f.good()) {
            PH_LOGE("Failed to open file %s : %s", filename.c_str(), errno2str(errno));
            return {};
        }
        return loadObj(f);
    }

private:
    void moveFrom(FatMesh & rhs) {
        if (this == &rhs) return;
        position = std::move(rhs.position);
        normal   = std::move(rhs.normal);
        texcoord = std::move(rhs.texcoord);
        tangent  = std::move(rhs.tangent);
        bbox     = rhs.bbox;
        rhs.bbox.setEmpty();
        PH_ASSERT(rhs.empty());
    }
};

} // namespace ph