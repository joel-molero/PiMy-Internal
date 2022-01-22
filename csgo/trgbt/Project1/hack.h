#pragma once

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

#define FLOAT32_NAN_BITS     (unsigned long)0x7FC00000
#define FLOAT32_NAN          BitsToFloat( FLOAT32_NAN_BITS )
#define BONE_USED_BY_HITBOX            0x00000100

#define VEC_T_NAN FLOAT32_NAN
#define Cabeza_i 0
#define Cuello_i 1
#define Pelvis_i 2
#define HombroD_i 3
#define CodoD_i 4
#define ManoD_i 5
#define HombroI_i 6
#define CodoI_i 7
#define ManoI_i 8
#define RodillaD_i 9
#define PieD_i 10
#define RodillaI_i 11
#define PieI_i 12
#define PI 3.14159265358979323846

typedef float vec_t;

class QAngleByValue;

class QAngle
{
public:
    // Members
    vec_t x, y, z;

    // Construction/destruction
    QAngle(void);
    QAngle(vec_t X, vec_t Y, vec_t Z);
    //	QAngle(RadianEuler const &angles);	// evil auto type promotion!!!

        // Allow pass-by-value
    operator QAngleByValue& () { return *((QAngleByValue*)(this)); }
    operator const QAngleByValue& () const { return *((const QAngleByValue*)(this)); }

    // Initialization
    void Init(vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f);
    void Random(vec_t minVal, vec_t maxVal);

    // Got any nasty NAN's?
    bool IsValid() const;
    void Invalidate();

    // array access...
    vec_t operator[](int i) const;
    vec_t& operator[](int i);

    // Base address...
    vec_t* Base();
    vec_t const* Base() const;

    // equality
    bool operator==(const QAngle& v) const;
    bool operator!=(const QAngle& v) const;

    // arithmetic operations
    QAngle& operator+=(const QAngle& v);
    QAngle& operator-=(const QAngle& v);
    QAngle& operator*=(float s);
    QAngle& operator/=(float s);

    // Get the vector's magnitude.
    vec_t	Length() const;
    vec_t	LengthSqr() const;

    // negate the QAngle components
    //void	Negate(); 

    // No assignment operators either...
    QAngle& operator=(const QAngle& src);

#ifndef VECTOR_NO_SLOW_OPERATIONS
    // copy constructors

    // arithmetic operations
    QAngle	operator-(void) const;

    QAngle	operator+(const QAngle& v) const;
    QAngle	operator-(const QAngle& v) const;
    QAngle	operator*(float fl) const;
    QAngle	operator/(float fl) const;
#else

private:
    // No copy constructors allowed if we're in optimal mode
    QAngle(const QAngle& vOther);

#endif
};

class QAngleByValue : public QAngle
{
public:
    // Construction/destruction:
    QAngleByValue(void) : QAngle() {}
    QAngleByValue(vec_t X, vec_t Y, vec_t Z) : QAngle(X, Y, Z) {}
    QAngleByValue(const QAngleByValue& vOther) { *this = vOther; }
};

class Quaternion;

class RadianEuler
{
public:
    inline RadianEuler(void) { }
    inline RadianEuler(vec_t X, vec_t Y, vec_t Z) { x = X; y = Y; z = Z; }
    inline RadianEuler(Quaternion const& q);	// evil auto type promotion!!!
    inline RadianEuler(QAngle const& angles);	// evil auto type promotion!!!

    // Initialization
    inline void Init(vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f) { x = ix; y = iy; z = iz; }

    //	conversion to qangle
    QAngle ToQAngle(void) const;
    bool IsValid() const;
    void Invalidate();

    // array access...
    vec_t operator[](int i) const;
    vec_t& operator[](int i);

    vec_t x, y, z;
};

class Quaternion				// same data-layout as engine's vec4_t,
{								//		which is a vec_t[4]
public:
    inline Quaternion(void) {

        // Initialize to NAN to catch errors
#ifdef _DEBUG
#ifdef VECTOR_PARANOIA
        x = y = z = w = VEC_T_NAN;
#endif
#endif
    }
    inline Quaternion(vec_t ix, vec_t iy, vec_t iz, vec_t iw) : x(ix), y(iy), z(iz), w(iw) { }
    inline Quaternion(RadianEuler const& angle);	// evil auto type promotion!!!

    inline void Init(vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f, vec_t iw = 0.0f) { x = ix; y = iy; z = iz; w = iw; }

    bool IsValid() const;
    void Invalidate();

    bool operator==(const Quaternion& src) const;
    bool operator!=(const Quaternion& src) const;

    vec_t* Base() { return (vec_t*)this; }
    const vec_t* Base() const { return (vec_t*)this; }

    // array access...
    vec_t operator[](int i) const;
    vec_t& operator[](int i);

    vec_t x, y, z, w;
};




inline vec_t BitsToFloat(unsigned long i)
{
    return *reinterpret_cast<vec_t*>(&i);
}

struct Vector
{
    Vector() {}
    Vector(float x1, float y1, float z1) { x = x1; y = y1; z = z1; }
    float x;
    float y;
    float z;
};

struct matrix3x4_t
{
    matrix3x4_t() {}
    matrix3x4_t(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23)
    {
        m_flMatVal[0][0] = m00;	m_flMatVal[0][1] = m01; m_flMatVal[0][2] = m02; m_flMatVal[0][3] = m03;
        m_flMatVal[1][0] = m10;	m_flMatVal[1][1] = m11; m_flMatVal[1][2] = m12; m_flMatVal[1][3] = m13;
        m_flMatVal[2][0] = m20;	m_flMatVal[2][1] = m21; m_flMatVal[2][2] = m22; m_flMatVal[2][3] = m23;
    }

    //-----------------------------------------------------------------------------
    // Creates a matrix where the X axis = forward
    // the Y axis = left, and the Z axis = up
    //-----------------------------------------------------------------------------
    void Init(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector& vecOrigin)
    {
        m_flMatVal[0][0] = xAxis.x; m_flMatVal[0][1] = yAxis.x; m_flMatVal[0][2] = zAxis.x; m_flMatVal[0][3] = vecOrigin.x;
        m_flMatVal[1][0] = xAxis.y; m_flMatVal[1][1] = yAxis.y; m_flMatVal[1][2] = zAxis.y; m_flMatVal[1][3] = vecOrigin.y;
        m_flMatVal[2][0] = xAxis.z; m_flMatVal[2][1] = yAxis.z; m_flMatVal[2][2] = zAxis.z; m_flMatVal[2][3] = vecOrigin.z;
    }

    //-----------------------------------------------------------------------------
    // Creates a matrix where the X axis = forward
    // the Y axis = left, and the Z axis = up
    //-----------------------------------------------------------------------------
    matrix3x4_t(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector& vecOrigin)
    {
        Init(xAxis, yAxis, zAxis, vecOrigin);
    }

    inline void Invalidate(void)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                m_flMatVal[i][j] = VEC_T_NAN;
            }
        }
    }

    float* operator[](int i) { assert((i >= 0) && (i < 3)); return m_flMatVal[i]; }
    const float* operator[](int i) const { assert((i >= 0) && (i < 3)); return m_flMatVal[i]; }
    float* Base() { return &m_flMatVal[0][0]; }
    const float* Base() const { return &m_flMatVal[0][0]; }

    float m_flMatVal[3][4];
};

struct mstudiobone_t
{
    int					sznameindex;
    inline char* const pszName(void) const { return ((char*)this) + sznameindex; }
    int		 			parent;		// parent bone
    int					bonecontroller[6];	// bone controller index, -1 == none

    // default values
    Vector				pos;
    Quaternion			quat;
    RadianEuler			rot;
    // compression scale
    Vector				posscale;
    Vector				rotscale;

    matrix3x4_t			poseToBone;
    Quaternion			qAlignment;
    int					flags;
    int					proctype;
    int					procindex;		// procedural rule
    mutable int			physicsbone;	// index into physically simulated bone
    inline void* pProcedure() const { if (procindex == 0) return NULL; else return  (void*)(((byte*)this) + procindex); };
    int					surfacepropidx;	// index into string tablefor property name
    inline char* const pszSurfaceProp(void) const { return ((char*)this) + surfacepropidx; }
    int					contents;		// See BSPFlags.h for the contents flags

    int					unused[8];		// remove as appropriate

    mstudiobone_t() {}
private:
    // No copy constructors allowed
    mstudiobone_t(const mstudiobone_t& vOther);
};



struct mstudiobbox_t {
    int bone;                 // hitbox bone
    int group;                // intersection group
    Vector bbmin;             // bounding box, or the ends of the capsule if flCapsuleRadius > 0 
    Vector bbmax;
    int szhitboxnameindex;    // offset to the name of the hitbox.
    int unused[3];
    float radius;    // capsule radius, -1 if box
    int pad[4];

    const char* pszHitboxName() const {
        if (szhitboxnameindex == 0)
            return "";

        return ((const char*)this) + szhitboxnameindex;
    }
};

struct mstudiohitboxset_t
{
    int					sznameindex;
    inline char* const	pszName(void) const { return ((char*)this) + sznameindex; }
    int					numhitboxes;
    int					hitboxindex;
    inline mstudiobbox_t* pHitbox(int i) const { return (mstudiobbox_t*)(((byte*)this) + hitboxindex) + i; };
};

struct studiohdr_t
{

    int         id;             // Model format ID, such as "IDST" (0x49 0x44 0x53 0x54)
    int         version;        // Format version number, such as 48 (0x30,0x00,0x00,0x00)
    int         checksum;       // This has to be the same in the phy and vtx files to load!
    char        name[64];       // The internal name of the model, padding with null bytes.
                                // Typically "my_model.mdl" will have an internal name of "my_model"
    int         dataLength;     // Data size of MDL file in bytes.

    // A vector is 12 bytes, three 4-byte float-values in a row.
    Vector      eyeposition;    // Position of player viewpoint relative to model origin
    Vector      illumposition;  // Position (relative to model origin) used to calculate ambient light contribution and cubemap reflections for the entire model.
    Vector      hull_min;       // Corner of model hull box with the least X/Y/Z values
    Vector      hull_max;        // Opposite corner of model hull box
    Vector      view_bbmin;     // TODO: what's this, how is it different from hull_min/max?
    Vector      view_bbmax;

    int         flags;          // Binary flags in little-endian order. 
                                // ex (00000001,00000000,00000000,11000000) means flags for position 0, 30, and 31 are set. 
                                // Set model flags section for more information

    /*
     * After this point, the header contains many references to offsets
     * within the MDL file and the number of items at those offsets.
     *
     * Offsets are from the very beginning of the file.
     *
     * Note that indexes/counts are not always paired and ordered consistently.
     */

     // mstudiobone_t
    int        bone_count;    // Number of data sections (of type mstudiobone_t)
    int        bone_offset;   // Offset of first data section
    inline mstudiobone_t* pBone(int i) const { 
        return (mstudiobone_t*)(((byte*)this) + bone_offset) + i; 
    }

    // mstudiobonecontroller_t
    int        bonecontroller_count;
    int        bonecontroller_offset;

    // mstudiohitboxset_t
    int        hitbox_count;
    int        hitbox_offset;
    mstudiohitboxset_t* pHitboxSet(int i) const
    {
        return (mstudiohitboxset_t*)(((byte*)this) + hitbox_offset) + i;
    };

    // mstudioanimdesc_t
    int        localanim_count;
    int        localanim_offset;

    // mstudioseqdesc_t
    int        localseq_count;
    int        localseq_offset;

    int        activitylistversion; // ??
    int        eventsindexed;       // ??

    // VMT texture filenames
    // mstudiotexture_t
    int        texture_count;
    int        texture_offset;

    // This offset points to a series of ints.
    // Each int value, in turn, is an offset relative to the start of this header/the-file,
    // At which there is a null-terminated string.
    int        texturedir_count;
    int        texturedir_offset;

    // Each skin-family assigns a texture-id to a skin location
    int        skinreference_count;
    int        skinrfamily_count;
    int        skinreference_index;

    // mstudiobodyparts_t
    int        bodypart_count;
    int        bodypart_offset;

    // Local attachment points        
    // mstudioattachment_t
    int        attachment_count;
    int        attachment_offset;

    // Node values appear to be single bytes, while their names are null-terminated strings.
    int        localnode_count;
    int        localnode_index;
    int        localnode_name_index;

    // mstudioflexdesc_t
    int        flexdesc_count;
    int        flexdesc_index;

    // mstudioflexcontroller_t
    int        flexcontroller_count;
    int        flexcontroller_index;

    // mstudioflexrule_t
    int        flexrules_count;
    int        flexrules_index;

    // IK probably referse to inverse kinematics
    // mstudioikchain_t
    int        ikchain_count;
    int        ikchain_index;

    // Information about any "mouth" on the model for speech animation
    // More than one sounds pretty creepy.
    // mstudiomouth_t
    int        mouths_count;
    int        mouths_index;

    // mstudioposeparamdesc_t
    int        localposeparam_count;
    int        localposeparam_index;

    /*
     * For anyone trying to follow along, as of this writing,
     * the next "surfaceprop_index" value is at position 0x0134 (308)
     * from the start of the file.
     */

     // Surface property value (single null-terminated string)
    int        surfaceprop_index;

    // Unusual: In this one index comes first, then count.
    // Key-value data is a series of strings. If you can't find
    // what you're interested in, check the associated PHY file as well.
    int        keyvalue_index;
    int        keyvalue_count;

    // More inverse-kinematics
    // mstudioiklock_t
    int        iklock_count;
    int        iklock_index;


    float      mass;      // Mass of object (4-bytes)
    int        contents;    // ??

    // Other models can be referenced for re-used sequences and animations
    // (See also: The $includemodel QC option.)
    // mstudiomodelgroup_t
    int        includemodel_count;
    int        includemodel_index;

    int        virtualModel;    // Placeholder for mutable-void*
    // Note that the SDK only compiles as 32-bit, so an int and a pointer are the same size (4 bytes)

    // mstudioanimblock_t
    int        animblocks_name_index;
    int        animblocks_count;
    int        animblocks_index;

    int        animblockModel; // Placeholder for mutable-void*

    // Points to a series of bytes?
    int        bonetablename_index;

    int        vertex_base;    // Placeholder for void*
    int        offset_base;    // Placeholder for void*

    // Used with $constantdirectionallight from the QC 
    // Model should have flag #13 set if enabled
    byte        directionaldotproduct;

    byte        rootLod;    // Preferred rather than clamped

    // 0 means any allowed, N means Lod 0 -> (N-1)
    byte        numAllowedRootLods;

    byte        unused0; // ??
    int         unused1; // ??

    // mstudioflexcontrollerui_t
    int         flexcontrollerui_count;
    int         flexcontrollerui_index;

    float       vertAnimFixedPointScale; // ??
    int         unused2;

    /**
     * Offset for additional header information.
     * May be zero if not present, or also 408 if it immediately
     * follows this studiohdr_t
     */
     // studiohdr2_t
    int         studiohdr2index;

    int         unused3; // ??

    /**
     * As of this writing, the header is 408 bytes long in total
     */
};



/*
struct Model_Name {
	const char* Name;
	Bone_Order* Bone_Distribution;
};
*/


struct Bone_Order{
	char Cabeza;
	char Cuello;
	char Pelvis;
	char HombroD;
	char CodoD;
	char ManoD;
	char HombroI;
	char CodoI;
	char ManoI;
	char RodillaD;
	char PieD;
	char RodillaI;
	char PieI;

};

struct Vec2 {
	float x, y;
};
struct Vec3 {
    Vec3() { x = 0; y = 0; z = 0; }
    Vec3(Vector v) { x = v.x; y = v.y; z = v.z; }
    Vec3(float x1, float y1, float z1) { x = x1; y = y1; z = z1; }
    Vec3& operator+(Vec3 arg)
    {
        x += arg.x;
        y += arg.y;
        z += arg.z;
        return *this;
    }
	float x, y, z;
};
struct Vec4 {
    Vec4() {}
    Vec4(float x1, float y1, float z1, float w1) { x = x1; y = y1; z = z1; w = w1; }
    Vec4(Vec3 p) { x = p.x; y = p.y; z = p.z; w = 1; }
	float x, y, z, w;
};

struct model_t {
	void* fnHandle;
	char szName[260];
};

class Ent {
public:
	union {
		DEFINE_MEMBER_N(bool, isDormant, 0xED);

		DEFINE_MEMBER_N(int, iHealth, 0x100);

		DEFINE_MEMBER_N(Vec3, vecOrigin, 0x138);

		DEFINE_MEMBER_N(int, iTeamNum, 0xF4);

		DEFINE_MEMBER_N(int, boneMatrix, 0x26A8);

		DEFINE_MEMBER_N(int, ArmorValue, 0x117CC);

		DEFINE_MEMBER_N(Vec3, aimPunchAngle, 0x303C);

        DEFINE_MEMBER_N(Vec3, vecVelocity, 0x114);

		//DEFINE_MEMBER_N(uintptr_t, studio_hdr, 0x2950);
	};
    /*
    int draw_model(int flags, uint8_t alpha) {
        using fn = int(__thiscall*)(void*, int, uint8_t);
        return util::get_vfunc< fn >(get_client_renderable(), 9)(get_client_renderable(), flags, alpha);
    }
    */
};

class EntitiesS
{
public:
    Ent* enemyEnts[5];
    Ent* alliedEnts[5];
};


struct EnemiesInfo
{
    EnemiesInfo()
    {

        /*enemieentity[0] = nullptr;
        enemieentity[1] = nullptr;
        enemieentity[2] = nullptr;
        enemieentity[3] = nullptr;
        enemieentity[4] = nullptr;*/
        enemieentity = nullptr;
    }
    //EnemiesInfo(Ent* enemieentity, int pos, bool z1) { 
      //  enemieentity[] = x1; pos = y1; rage = z1; }
    Ent** enemieentity;
    int* pos;
    bool* rage;
    int countPlayers;
    Ent* closestEntity;
};

class EntListObj {
public:
    struct Ent* ent;
    char padding[12];
};

class EntList {
public:
    EntListObj ents[32];
};


class IClientEntityList
{
public:
    // Get IClientNetworkable interface for specified entity
    virtual void* GetClientNetworkable(int entnum) = 0;
    virtual void* GetClientNetworkableFromHandle(int hEnt) = 0;
    virtual void* GetClientUnknownFromHandle(int hEnt) = 0;

    // NOTE: This function is only a convenience wrapper.
    // It returns GetClientNetworkable( entnum )->GetIClientEntity().
    virtual void* GetClientEntity(int entnum) = 0;
    virtual void* GetClientEntityFromHandle(int hEnt) = 0;

    // Returns number of entities currently in use
    virtual int					NumberOfEntities(bool bIncludeNonNetworkable) = 0;

    // Returns highest index actually used
    virtual int					GetHighestEntityIndex(void) = 0;

    // Sizes entity list to specified size
    virtual void				SetMaxEntities(int maxents) = 0;
    virtual int					GetMaxEntities() = 0;
};





template <typename Fn> __forceinline Fn GetVirtualFunction(void* pClassBase, int nFunctionIndex)
{
    return (Fn)((PDWORD) * (PDWORD*)pClassBase)[nFunctionIndex];
}

enum MaterialVarFlags_t
{
    MATERIAL_VAR_DEBUG = (1 << 0),
    MATERIAL_VAR_NO_DEBUG_OVERRIDE = (1 << 1),
    MATERIAL_VAR_NO_DRAW = (1 << 2),
    MATERIAL_VAR_USE_IN_FILLRATE_MODE = (1 << 3),

    MATERIAL_VAR_VERTEXCOLOR = (1 << 4),
    MATERIAL_VAR_VERTEXALPHA = (1 << 5),
    MATERIAL_VAR_SELFILLUM = (1 << 6),
    MATERIAL_VAR_ADDITIVE = (1 << 7),
    MATERIAL_VAR_ALPHATEST = (1 << 8),
    MATERIAL_VAR_MULTIPASS = (1 << 9),
    MATERIAL_VAR_ZNEARER = (1 << 10),
    MATERIAL_VAR_MODEL = (1 << 11),
    MATERIAL_VAR_FLAT = (1 << 12),
    MATERIAL_VAR_NOCULL = (1 << 13),
    MATERIAL_VAR_NOFOG = (1 << 14),
    MATERIAL_VAR_IGNOREZ = (1 << 15),
    MATERIAL_VAR_DECAL = (1 << 16),
    MATERIAL_VAR_ENVMAPSPHERE = (1 << 17),
    MATERIAL_VAR_NOALPHAMOD = (1 << 18),
    MATERIAL_VAR_ENVMAPCAMERASPACE = (1 << 19),
    MATERIAL_VAR_BASEALPHAENVMAPMASK = (1 << 20),
    MATERIAL_VAR_TRANSLUCENT = (1 << 21),
    MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = (1 << 22),
    MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = (1 << 23),
    MATERIAL_VAR_OPAQUETEXTURE = (1 << 24),
    MATERIAL_VAR_ENVMAPMODE = (1 << 25),
    MATERIAL_VAR_SUPPRESS_DECALS = (1 << 26),
    MATERIAL_VAR_HALFLAMBERT = (1 << 27),
    MATERIAL_VAR_WIREFRAME = (1 << 28),
    MATERIAL_VAR_ALLOWALPHATOCOVERAGE = (1 << 29),
    MATERIAL_VAR_IGNORE_ALPHA_MODULATION = (1 << 30),

    // NOTE: Only add flags here that either should be read from
    // .vmts or can be set directly from client code. Other, internal
    // flags should to into the flag enum in imaterialinternal.h
};

enum ImageFormat
{
    IMAGE_FORMAT_UNKNOWN = -1,
    IMAGE_FORMAT_RGBA8888 = 0,
    IMAGE_FORMAT_ABGR8888,
    IMAGE_FORMAT_RGB888,
    IMAGE_FORMAT_BGR888,
    IMAGE_FORMAT_RGB565,
    IMAGE_FORMAT_I8,
    IMAGE_FORMAT_IA88,
    IMAGE_FORMAT_P8,
    IMAGE_FORMAT_A8,
    IMAGE_FORMAT_RGB888_BLUESCREEN,
    IMAGE_FORMAT_BGR888_BLUESCREEN,
    IMAGE_FORMAT_ARGB8888,
    IMAGE_FORMAT_BGRA8888,
    IMAGE_FORMAT_DXT1,
    IMAGE_FORMAT_DXT3,
    IMAGE_FORMAT_DXT5,
    IMAGE_FORMAT_BGRX8888,
    IMAGE_FORMAT_BGR565,
    IMAGE_FORMAT_BGRX5551,
    IMAGE_FORMAT_BGRA4444,
    IMAGE_FORMAT_DXT1_ONEBITALPHA,
    IMAGE_FORMAT_BGRA5551,
    IMAGE_FORMAT_UV88,
    IMAGE_FORMAT_UVWQ8888,
    IMAGE_FORMAT_RGBA16161616F,
    IMAGE_FORMAT_RGBA16161616,
    IMAGE_FORMAT_UVLX8888,
    IMAGE_FORMAT_R32F,			// Single-channel 32-bit floating point
    IMAGE_FORMAT_RGB323232F,
    IMAGE_FORMAT_RGBA32323232F,

    // Depth-stencil texture formats for shadow depth mapping
    IMAGE_FORMAT_NV_DST16,		// 
    IMAGE_FORMAT_NV_DST24,		//
    IMAGE_FORMAT_NV_INTZ,		// Vendor-specific depth-stencil texture
    IMAGE_FORMAT_NV_RAWZ,		// formats for shadow depth mapping 
    IMAGE_FORMAT_ATI_DST16,		// 
    IMAGE_FORMAT_ATI_DST24,		//
    IMAGE_FORMAT_NV_NULL,		// Dummy format which takes no video memory

    // Compressed normal map formats
    IMAGE_FORMAT_ATI2N,			// One-surface ATI2N / DXN format
    IMAGE_FORMAT_ATI1N,			// Two-surface ATI1N format

#if defined( _X360 )
    // Depth-stencil texture formats
    IMAGE_FORMAT_X360_DST16,
    IMAGE_FORMAT_X360_DST24,
    IMAGE_FORMAT_X360_DST24F,
    // supporting these specific formats as non-tiled for procedural cpu access
    IMAGE_FORMAT_LINEAR_BGRX8888,
    IMAGE_FORMAT_LINEAR_RGBA8888,
    IMAGE_FORMAT_LINEAR_ABGR8888,
    IMAGE_FORMAT_LINEAR_ARGB8888,
    IMAGE_FORMAT_LINEAR_BGRA8888,
    IMAGE_FORMAT_LINEAR_RGB888,
    IMAGE_FORMAT_LINEAR_BGR888,
    IMAGE_FORMAT_LINEAR_BGRX5551,
    IMAGE_FORMAT_LINEAR_I8,
    IMAGE_FORMAT_LINEAR_RGBA16161616,

    IMAGE_FORMAT_LE_BGRX8888,
    IMAGE_FORMAT_LE_BGRA8888,
#endif

    NUM_IMAGE_FORMATS
};


struct ModelRenderInfo_t
{
    Vector origin;
    QAngle angles;
    char   pad[0x4];
    void* pRenderable;
    const model_t* pModel;
    const void* pModelToWorld;
    const void* pLightingOffset;
    const void* pLightingOrigin;
    int flags;
    int entity_index;
    int skin;
    int body;
    int hitboxset;
    uintptr_t instance;
    ModelRenderInfo_t()
    {
        pModelToWorld = NULL;
        pLightingOffset = NULL;
        pLightingOrigin = NULL;
    }
};

struct DrawModelState_t
{
    void* m_pStudioHdr;
    void* m_pStudioHWData;
    void* m_pRenderable;
    const void* m_pModelToWorld;
    uintptr_t        m_decals;
    int                        m_drawFlags;
    int                        m_lod;
};
class IMaterialVar
{
private:
public:
    void SetFloatValue(float value)
    {
        GetVirtualFunction< void(__thiscall*)(decltype(this), float) >(this, 4)(this, value);
    }

    void SetVectorValue(float r, float g, float b)
    {
        GetVirtualFunction< void(__thiscall*)(decltype(this), float, float, float) >(this, 11)(this, r, g, b);
    }

    void SetStringValue(char const* value)
    {
        GetVirtualFunction< void(__thiscall*)(decltype(this), char const*) >(this, 6)(this, value);
    }
};


class IMaterial
{
public:
    // Get the name of the material.  This is a full path to
    // the vmt file starting from "hl2/materials" (or equivalent) without
    // a file extension.
    virtual const char* GetName() const = 0;
    virtual const char* GetTextureGroupName() const = 0;

    // Get the preferred size/bitDepth of a preview image of a material.
    // This is the sort of image that you would use for a thumbnail view
    // of a material, or in WorldCraft until it uses materials to render.
    // separate this for the tools maybe
    virtual int/*PreviewImageRetVal_t*/ get_preview_image_properties(int* width, int* height,
        ImageFormat* imageFormat,
        bool* isTranslucent) const = 0;

    // Get a preview image at the specified width/height and bitDepth.
    // Will do resampling if necessary.(not yet!!! :) )
    // Will do color format conversion. (works now.)
    virtual int /*PreviewImageRetVal_t*/ get_preview_image(unsigned char* data,
        int width, int height,
        ImageFormat imageFormat) const = 0;
    //
    virtual int get_mapping_width() = 0;
    virtual int get_mapping_height() = 0;

    virtual int get_num_animation_frames() = 0;

    // For material subrects (material pages).  Offset(u,v) and scale(u,v) are normalized to texture.
    virtual bool in_material_page(void) = 0;
    virtual void get_material_offset(float* pOffset) = 0;
    virtual void get_material_scale(float* pScale) = 0;
    virtual IMaterial* get_material_page(void) = 0;

    // find a vmt variable.
    // This is how game code affects how a material is rendered.
    // The game code must know about the params that are used by
    // the shader for the material that it is trying to affect.
    virtual IMaterialVar* find_var(const char* varName, bool* found, bool complain = true) = 0;

    // The user never allocates or deallocates materials.  Reference counting is
    // used instead.  Garbage collection is done upon a call to
    // i_material_system::UncacheUnusedMaterials.
    virtual void increment_reference_count(void) = 0;
    virtual void decrement_reference_count(void) = 0;

    inline void add_ref()
    {
        increment_reference_count();
    }

    inline void release()
    {
        decrement_reference_count();
    }

    // Each material is assigned a number that groups it with like materials
    // for sorting in the application.
    virtual int get_enumeration_id(void) const = 0;

    virtual void get_low_res_color_sample(float s, float t, float* color) const = 0;

    // This computes the state snapshots for this material
    virtual void recompute_state_snapshots() = 0;

    // Are we translucent?
    virtual bool is_translucent() = 0;

    // Are we alphatested?
    virtual bool is_alpha_tested() = 0;

    // Are we vertex lit?
    virtual bool is_vertex_lit() = 0;

    // Gets the vertex format
    virtual void get_vertex_format() const = 0;

    // returns true if this material uses a material proxy
    virtual bool has_proxy(void) const = 0;

    virtual bool uses_env_cubemap(void) = 0;

    virtual bool needs_tangent_space(void) = 0;

    virtual bool needs_power_of_two_frame_buffer_texture(bool bCheckSpecificToThisFrame = true) = 0;
    virtual bool needs_full_frame_buffer_texture(bool bCheckSpecificToThisFrame = true) = 0;

    // returns true if the shader doesn't do skinning itself and requires
    // the data that is sent to it to be preskinned.
    virtual bool needs_software_skinning(void) = 0;

    // Apply constant color or alpha modulation
    virtual void alpha_modulate(float alpha) = 0;
    virtual void color_modulate(float r, float g, float b) = 0;

    // Material Var flags...
    virtual void set_matrial_var_flag(MaterialVarFlags_t flag, bool on) = 0; // Not used, index outdated, see below
    virtual bool GetMaterialVarFlag(MaterialVarFlags_t flag) const = 0;

    // Gets material reflectivity
    virtual void get_reflectivity(Vec3& reflect) = 0;

    // Gets material property flags
    virtual bool get_property_flag(int /*MaterialPropertyTypes_t*/ type) = 0;

    // Is the material visible from both sides?
    virtual bool is_two_sided() = 0;

    // Sets the shader associated with the material
    virtual void set_shader(const char* pShaderName) = 0;

    // Can't be const because the material might have to precache itself.
    virtual int get_num_passes(void) = 0;

    // Can't be const because the material might have to precache itself.
    virtual int get_texture_memory_bytes(void) = 0;

    // Meant to be used with materials created using CreateMaterial
    // It updates the materials to reflect the current values stored in the material vars
    virtual void refresh() = 0;

    // GR - returns true is material uses lightmap alpha for blending
    virtual bool needs_lightmap_blend_alpha(void) = 0;

    // returns true if the shader doesn't do lighting itself and requires
    // the data that is sent to it to be prelighted
    virtual bool needs_software_lighting(void) = 0;

    // Gets at the shader parameters
    virtual int shader_param_count() const = 0;
    virtual IMaterialVar** get_shader_params(void) = 0;

    // Returns true if this is the error material you get back from i_material_system::FindMaterial if
    // the material can't be found.
    virtual bool IsErrorMaterial() const = 0;

    virtual void Unused() = 0;

    // Gets the current alpha modulation
    virtual float GetAlphaModulation() = 0;
    virtual void GetColorModulation(float* r, float* g, float* b) = 0;

    // Is this translucent given a particular alpha modulation?
    virtual bool is_translucent_under_modulation(float fAlphaModulation = 1.0f) const = 0;

    // fast find that stores the index of the found var in the string table in local cache
    virtual IMaterialVar* find_var_fast(char const* pVarName, unsigned int* pToken) = 0;

    // Sets new VMT shader parameters for the material
    virtual void set_shader_and_params(void* pKeyValues) = 0;
    virtual const char* get_shader_name() const = 0;

    virtual void delete_if_unreferenced() = 0;

    virtual bool is_sprite_card() = 0;

    virtual void call_bind_proxy(void* proxyData) = 0;

    virtual void refresh_preserving_material_vars() = 0;

    virtual bool was_reloaded_from_whitelist() = 0;

    virtual bool set_temp_excluded(bool bSet, int nExcludedDimensionLimit) = 0;

    virtual int get_reference_count() const = 0;

    void AlphaModulate(float alpha)
    {
        return GetVirtualFunction< void(__thiscall*)(decltype(this), float) >(this, 27)(this, alpha);
    }

    void SetMaterialVarFlag(MaterialVarFlags_t flag, bool on)
    {
        return GetVirtualFunction< void(__thiscall*)(decltype(this), MaterialVarFlags_t, bool) >(this, 29)(this, flag, on);
    }

    void ColorModulate(const float color[3])
    {
        return GetVirtualFunction< void(__thiscall*)(decltype(this), float, float, float)>(this, 28)(this, color[0], color[1], color[2]);
    }
};


class IVModelRender
{
public:
    virtual int DrawModel(int flags, void* pRenderable, uintptr_t instance, int entity_index,
        const model_t* model, Vec3 const& origin, Vec3 const& angles, int skin, int body,
        int hitboxset, const void* modelToWorld = NULL,
        const void* pLightingOffset = NULL) = 0;

    virtual void ForcedMaterialOverride(IMaterial* newMaterial, int nOverrideType = 0, int nOverrides = 0) = 0;
    virtual bool IsForcedMaterialOverride(void) = 0;
    virtual void SetViewTarget(const int* pStudioHdr, int nBodyIndex, const Vec3& target) = 0;
    virtual uintptr_t CreateInstance(void* pRenderable, void* pCache = NULL) = 0;
    virtual void DestroyInstance(uintptr_t handle) = 0;
};

class IVModelInfoClient
{
public:
    inline void* GetModel(int Index)
    {
        return GetVirtualFunction<void* (__thiscall*)(void*, int)>(this, 1)(this, Index);
    }

    inline int GetModelIndex(const char* Filename)
    {
        return GetVirtualFunction<int(__thiscall*)(void*, const char*)>(this, 2)(this, Filename);
    }

    inline const char* GetModelName(const void* Model)
    {
        return GetVirtualFunction<const char* (__thiscall*)(void*, const void*)>(this, 3)(this, Model);
    }
    inline studiohdr_t* GetStudioModel(const model_t* model)
    {
        return GetVirtualFunction<studiohdr_t* (__thiscall*)(decltype(this), const model_t*) >(this, 32)(this, model);
    }
    void GetModelMaterials(const model_t* model, int count, IMaterial** ppMaterial)
    {
        GetVirtualFunction<void(__thiscall*)(decltype(this), const model_t*, int, IMaterial**) >(this, 18)(this, model, count, ppMaterial);
    }
};



class IMaterialSystem
{
public:
    const char* GetName()
    {
        return GetVirtualFunction< const char* (__thiscall*)(decltype(this)) >(this, 0)(this);
    }

    IMaterial* CreateMaterial(const char* material_name, void* kv)
    {
        return GetVirtualFunction< IMaterial* (__thiscall*)(decltype(this), const char*, void*) >(this, 83)(
            this, material_name, kv);
    }

    IMaterial* FindMaterial(const char* material_name, const char* texture_group_name = nullptr, bool complain = true, const char* complain_prefix = nullptr)
    {
        return GetVirtualFunction< IMaterial* (__thiscall*)(decltype(this), const char*, const char*, bool, const char*) >(this, 84)(this, material_name, texture_group_name, complain, complain_prefix);
    }

    int FirstMaterial()
    {
        return GetVirtualFunction< int(__thiscall*)(decltype(this)) >(this, 86)(this);
    }

    int NextMaterial(int handle)
    {
        return GetVirtualFunction< int(__thiscall*)(decltype(this), int) >(this, 87)(this, handle);
    }

    int InvalidMaterial()
    {
        return GetVirtualFunction< int(__thiscall*)(decltype(this)) >(this, 88)(this);
    }

    IMaterial* GetMaterial(short handle)
    {
        return GetVirtualFunction< IMaterial* (__thiscall*)(decltype(this), short) >(this, 89)(this, handle);
    }

    void* FindTexture(char const* pTextureName, const char* pTextureGroupName, bool complain = true)
    {
        return GetVirtualFunction< void* (__thiscall*)(decltype(this), char const*, const char*, bool)
        >(this, 91)(this, pTextureName, pTextureGroupName, complain);
    }

    void* GetRenderContext()
    {
        return GetVirtualFunction< void* (__thiscall*)(decltype(this)) >(this, 115)(this);
    }
};



class Hack {
public:
	uintptr_t dwEntityList = 0x4DD1E1C;
	uintptr_t dwViewMatrix = 0x4DC3734;
	uintptr_t dwLocalPlayer = 0xDB65EC;
	uintptr_t m_fFlags = 0x104;
    uintptr_t m_dwBoneMatrix = 0x26A8;
    uintptr_t m_vecVelocity = 0x114;
    uintptr_t dwClientState = 0x589FC4;
    uintptr_t dwClientState_ViewAngles = 0x4D90;
    uintptr_t m_vecOrigin = 0x138;
    uintptr_t m_vecViewOffset = 0x108;
    uintptr_t m_aimPunchAngle = 0x303C;
    uintptr_t m_iCrosshairId = 0x11838;

	uintptr_t engine;
	uintptr_t client;
	uintptr_t* getLocalPlayer;
    uintptr_t* clientState;
	//uintptr_t* player;
	Ent* localEnt;
	EntList* entList;
    EnemiesInfo enemies_list;
    EntitiesS* entitiesE;


	ID3DXLine* LineL;

	//Bone_Order Bones_Orders[9];
	//Model_Name Model_names[14];

	Vec2 crosshair2D;
	int crosshairSizeL = 7;
	int crosshairSizeR = 7;
	int crosshairSizeT = 7;
	int crosshairSizeB = 7;
    bool wa;
    clock_t time, time2;

	float viewMatrix[16];

	int Bones[13];

    Vec3** halfSphere0;
    Vec3** halfSphere1;

    Vec3 GetMyPos();
	void Bunny();
	void Init();
	void Update(void* ModelRenderVMT, IVModelRender* ModelRender);
    void EntityListReload();
    void Trigger();
	bool CheckValidEnt(Ent* ent);
    bool CheckValidEntStart(Ent* ent);
    bool CheckIfLocalEnt(Ent* ent);
	bool WorldToScreen(Vec3 pos, Vec2& screen);
    bool WorldToScreen(Vec3& pos);
	Vec3 GetBonePos(Ent* ent, int bone);
    Vec3 GetEnemyVel(Ent* ent);
    Vec3* GetCurrentAngles();
    Vec3* GetOrigin();
    Vec3* GetViewOffset();
    Vec3* GetPunchAngle();
    //int FindClosestEnemy(Vec3* final);
    int FindClosestEnemyToCrosshair();
    void AimBot(int position);
    void GetCurrentEntities(IClientEntityList* ClientEntityList);

};