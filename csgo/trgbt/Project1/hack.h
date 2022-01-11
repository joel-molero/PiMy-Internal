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

class EntListObj {
public:
	struct Ent* ent;
	char padding[12];
};

class EntList {
public:
	EntListObj ents[32];
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


class Hack {
public:
	uintptr_t dwEntityList = 0x4DD0AB4;
	uintptr_t dwViewMatrix = 0x4DC23B4;
	uintptr_t dwLocalPlayer = 0xDB558C;
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

	ID3DXLine* LineL;

	//Bone_Order Bones_Orders[9];
	//Model_Name Model_names[14];

	Vec2 crosshair2D;
	int crosshairSizeL = 16;
	int crosshairSizeR = 16;
	int crosshairSizeT = 16;
	int crosshairSizeB = 16;
    bool wa;
    clock_t time, time2;

	float viewMatrix[16];

	int Bones[13];

    Vec3** halfSphere0;
    Vec3** halfSphere1;

    Vec3 GetMyPos();
	void Bunny();
	void Init();
	void Update();
    void EntityListReload();
    void Trigger();
	bool CheckValidEnt(Ent* ent);
    bool CheckValidEntStart(Ent* ent);
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

};