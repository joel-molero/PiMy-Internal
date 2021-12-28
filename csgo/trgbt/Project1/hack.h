#pragma once

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}


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

struct Vector
{
    float aux[3];
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

    // mstudiobonecontroller_t
    int        bonecontroller_count;
    int        bonecontroller_offset;

    // mstudiohitboxset_t
    int        hitbox_count;
    int        hitbox_offset;

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
	float x, y, z;
};
struct Vec4 {
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

		//DEFINE_MEMBER_N(uintptr_t, studio_hdr, 0x2950);
	};
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

class Hack {
public:
	uintptr_t dwEntityList = 0x4DD0AB4;
	uintptr_t dwViewMatrix = 0x4DC23B4;
	uintptr_t dwLocalPlayer = 0xDB558C;
	uintptr_t m_fFlags = 0x104;

	uintptr_t engine;
	uintptr_t client;
	uintptr_t* getLocalPlayer;
	//uintptr_t* player;
	Ent* localEnt;
	EntList* entList;

	ID3DXLine* LineL;

	//Bone_Order Bones_Orders[9];
	//Model_Name Model_names[14];

	Vec2 crosshair2D;
	int crosshairSizeL = 44;
	int crosshairSizeR = 44;
	int crosshairSizeT = 44;
	int crosshairSizeB = 44;

	float viewMatrix[16];

	int Bones[13];

	void Bunny();
	void Init();
	void Update();
	bool CheckValidEnt(Ent* ent);
	bool WorldToScreen(Vec3 pos, Vec2& screen);
	Vec3 GetBonePos(Ent* ent, int bone);

};