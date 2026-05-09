// Copyright (C) 1999-2005 ID Software, Inc.
// OpenSandbox by noire.dev — GPLv2

#include "javascript.h"

float AngleDifference(float ang1, float ang2) {
	float diff;

	diff = ang1 - ang2;
	if(ang1 > ang2) {
		if(diff > 180.0) diff -= 360.0;
	} else {
		if(diff < -180.0) diff += 360.0;
	}
	return diff;
}

void StringCopy(char *dest, const char *source, int destsize) {
	if(!dest) {
		print("StringCopy: NULL dest \n");
		return;
	}
	if(!source) {
	    print("StringCopy: NULL source \n");
		return;
	}
	if(destsize < 1) {
		print("StringCopy: destsize < 1 \n");
		return;
	}

	strncpy(dest, source, destsize - 1);
	dest[destsize - 1] = 0;
}

int Q_stricmpn(const char *s1, const char *s2, int n) {
	int c1, c2;

	if(s1 == NULL) {
		if(s2 == NULL) return 0;
		else return -1;
	} else if(s2 == NULL) return 1;

	do {
		c1 = *s1++;
		c2 = *s2++;

		if(!n--) return 0; // strings are equal until end point

		if(c1 != c2) {
			if(c1 >= 'a' && c1 <= 'z') c1 -= ('a' - 'A');
			if(c2 >= 'a' && c2 <= 'z') c2 -= ('a' - 'A');
			if(c1 != c2) return c1 < c2 ? -1 : 1;
		}
	} while(c1);

	return 0; // strings are equal
}

int Q_strncmp(const char *s1, const char *s2, int n) {
	int c1, c2;

	do {
		c1 = *s1++;
		c2 = *s2++;

		if(!n--) {
			return 0; // strings are equal until end point
		}

		if(c1 != c2) {
			return c1 < c2 ? -1 : 1;
		}
	} while(c1);

	return 0; // strings are equal
}

int Q_stricmp(const char *s1, const char *s2) { return (s1 && s2) ? Q_stricmpn(s1, s2, 99999) : -1; }

char *Q_strlwr(char *s1) {
	char *s;

	s = s1;
	while(*s) {
		*s = tolower(*s);
		s++;
	}
	return s1;
}

char *Q_strupr(char *s1) {
	char *s;

	s = s1;
	while(*s) {
		*s = toupper(*s);
		s++;
	}
	return s1;
}

// never goes past bounds or leaves without a terminating 0
void Q_strcat(char *dest, int size, const char *src) {
	int l1;

	l1 = strlen(dest);
	iferr(l1 >= size);
	StringCopy(dest + l1, src, size - l1);
}

static	char	com_token[MAX_TOKEN_CHARS];
static	int		com_lines;

static char *SkipWhitespace( char *data, qboolean *hasNewLines ) {
	int c;

	while( (c = *data) <= ' ') {
		if( !c ) {
			return NULL;
		}
		if( c == '\n' ) {
			com_lines++;
			*hasNewLines = qtrue;
		}
		data++;
	}

	return data;
}

char *Q3Parse(char **data_p) {
	int c = 0, len;
	qboolean hasNewLines = qfalse;
	char *data;

	data = *data_p;
	len = 0;
	com_token[0] = 0;

	// make sure incoming data is valid
	if ( !data )
	{
		*data_p = NULL;
		return com_token;
	}

	while ( 1 )
	{
		// skip whitespace
		data = SkipWhitespace( data, &hasNewLines );
		if ( !data )
		{
			*data_p = NULL;
			return com_token;
		}

		c = *data;

		// skip double slash comments
		if ( c == '/' && data[1] == '/' )
		{
			data += 2;
			while (*data && *data != '\n') {
				data++;
			}
		}
		// skip /* */ comments
		else if ( c=='/' && data[1] == '*' ) 
		{
			data += 2;
			while ( *data && ( *data != '*' || data[1] != '/' ) ) 
			{
				data++;
			}
			if ( *data ) 
			{
				data += 2;
			}
		}
		else
		{
			break;
		}
	}

	// handle quoted strings
	if (c == '\"')
	{
		data++;
		while (1)
		{
			c = *data++;
			if (c=='\"' || !c)
			{
				com_token[len] = 0;
				*data_p = ( char * ) data;
				return com_token;
			}
			if (len < MAX_TOKEN_CHARS)
			{
				com_token[len] = c;
				len++;
			}
		}
	}

	// parse a regular word
	do
	{
		if (len < MAX_TOKEN_CHARS)
		{
			com_token[len] = c;
			len++;
		}
		data++;
		c = *data;
		if ( c == '\n' )
			com_lines++;
	} while (c>32);

	if (len == MAX_TOKEN_CHARS)
	{
	    len = 0;
	}
	com_token[len] = 0;

	*data_p = ( char * ) data;
	return com_token;
}

void QDECL Com_sprintf(char *dest, int size, const char *fmt, ...) {
	int len;
	va_list argptr;
	char bigbuffer[32000]; // big, but small enough to fit in PPC stack

	va_start(argptr, fmt);
	len = Q_vsnprintf(bigbuffer, sizeof(bigbuffer), fmt, argptr);
	va_end(argptr);
	iferr(len >= sizeof(bigbuffer));
	if(len >= size) {
		print("Com_sprintf: overflow of %i in %i\n", len, size);
	}
	StringCopy(dest, bigbuffer, size);
}

char *QDECL va(char *format, ...) {
	va_list argptr;
	static char string[4096][256];
    static int index = 0;
    
    char *buf = string[index];
    index = (index + 1) % 4096;

	va_start(argptr, format);
	Q_vsnprintf(buf, sizeof(string[0]), format, argptr);
	va_end(argptr);

	return buf;
}

char *Info_ValueForKey(const char *s, const char *key) {
	char pkey[BIG_INFO_KEY];
	static char value[2][BIG_INFO_VALUE]; // use two buffers so compares
	                                      // work without stomping on each other
	static int valueindex = 0;
	char *o;

	if(!s || !key) {
		return "";
	}

	iferr(strlen(s) >= BIG_INFO_STRING);

	valueindex ^= 1;
	if(*s == '\\') s++;
	while(1) {
		o = pkey;
		while(*s != '\\') {
			if(!*s) return "";
			*o++ = *s++;
		}
		*o = 0;
		s++;

		o = value[valueindex];

		while(*s != '\\' && *s) {
			*o++ = *s++;
		}
		*o = 0;

		if(!Q_stricmp(key, pkey)) return value[valueindex];

		if(!*s) break;
		s++;
	}

	return "";
}

void Info_RemoveKey(char *s, const char *key) {
	char *start;
	char pkey[MAX_INFO_KEY];
	char value[MAX_INFO_VALUE];
	char *o;

	iferr(strlen(s) >= MAX_INFO_STRING);

	if(strchr(key, '\\')) {
		return;
	}

	while(1) {
		start = s;
		if(*s == '\\') s++;
		o = pkey;
		while(*s != '\\') {
			if(!*s) return;
			*o++ = *s++;
		}
		*o = 0;
		s++;

		o = value;
		while(*s != '\\' && *s) {
			if(!*s) return;
			*o++ = *s++;
		}
		*o = 0;

		if(!strcmp(key, pkey)) {
			memmove(start, s, strlen(s) + 1); // remove this part

			return;
		}

		if(!*s) return;
	}
}

qboolean Info_Validate(const char *s) {
	if(strchr(s, '\"')) return qfalse;
	if(strchr(s, ';')) return qfalse;
	return qtrue;
}

void Info_SetValueForKey(char *s, const char *key, const char *value) {
	char newi[MAX_INFO_STRING];
	const char *blacklist = "\\;\"";

	iferr(strlen(s) >= MAX_INFO_STRING);

	for(; *blacklist; ++blacklist) {
		if(strchr(key, *blacklist) || strchr(value, *blacklist)) {
			print(S_COLOR_YELLOW "Can't use keys or values with a '%c': %s = %s\n", *blacklist, key, value);
			return;
		}
	}

	Info_RemoveKey(s, key);
	if(!value || !strlen(value)) return;

	Com_sprintf(newi, sizeof(newi), "\\%s\\%s", key, value);

	if(strlen(newi) + strlen(s) >= MAX_INFO_STRING) {
		print("Info string length exceeded\n");
		return;
	}

	strcat(newi, s);
	strcpy(s, newi);
}

void QDECL print(const char *msg, ...) {
	va_list argptr;
	char text[2048];

	va_start(argptr, msg);
	Q_vsnprintf(text, sizeof(text), msg, argptr);
	va_end(argptr);

	trap_Print(text);
}

#ifdef GAME
void QDECL broadcast(const char *msg, ...) {
	va_list argptr;
	char text[2048];

	va_start(argptr, msg);
	Q_vsnprintf(text, sizeof(text), msg, argptr);
	va_end(argptr);

	trap_SendServerCommand(-1, va("print \"%s\n\"", text));
}
#endif

void QDECL error(const char *msg, ...) {
	va_list argptr;
	char text[2048];

	va_start(argptr, msg);
	Q_vsnprintf(text, sizeof(text), msg, argptr);
	va_end(argptr);

	trap_Error(text);
}

int ST_GetPlayerMove(int buttons, int type) {
	if(type == PLAYER_MOVE_F && buttons & BMOVE_W && buttons & BMOVE_S) return 0;
	else if(type == PLAYER_MOVE_F && buttons & BMOVE_W) return 127;
	else if(type == PLAYER_MOVE_F && buttons & BMOVE_S) return -127;
	
	if(type == PLAYER_MOVE_R && buttons & BMOVE_A && buttons & BMOVE_D) return 0;
	else if(type == PLAYER_MOVE_R && buttons & BMOVE_A) return -127;
	else if(type == PLAYER_MOVE_R && buttons & BMOVE_D) return 127;
	
	if(type == PLAYER_MOVE_U && buttons & BMOVE_J && buttons & BMOVE_C) return 0;
	else if(type == PLAYER_MOVE_U && buttons & BMOVE_J) return 127;
	else if(type == PLAYER_MOVE_U && buttons & BMOVE_C) return -127;
	
	return 0;
}

#ifdef QVM
#define __QVM_MATH
#endif

vec3_t vec3_origin = {0, 0, 0};
vec3_t axisDefault[3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};

vec4_t g_color_table[9] = {
    {0.0, 0.0, 0.0, 1.0},
    {1.0, 0.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    {1.0, 1.0, 0.0, 1.0},
    {0.0, 0.0, 1.0, 1.0},
    {0.0, 1.0, 1.0, 1.0},
    {1.0, 0.0, 1.0, 1.0},
    {1.0, 1.0, 1.0, 1.0},
    {1.0, 0.5, 0.0, 1.0},
};

vec3_t bytedirs[NUMVERTEXNORMALS] = {{-0.525731f, 0.000000f, 0.850651f}, {-0.442863f, 0.238856f, 0.864188f}, {-0.295242f, 0.000000f, 0.955423f}, {-0.309017f, 0.500000f, 0.809017f}, {-0.162460f, 0.262866f, 0.951056f}, {0.000000f, 0.000000f, 1.000000f}, {0.000000f, 0.850651f, 0.525731f}, {-0.147621f, 0.716567f, 0.681718f}, {0.147621f, 0.716567f, 0.681718f}, {0.000000f, 0.525731f, 0.850651f}, {0.309017f, 0.500000f, 0.809017f}, {0.525731f, 0.000000f, 0.850651f}, {0.295242f, 0.000000f, 0.955423f}, {0.442863f, 0.238856f, 0.864188f}, {0.162460f, 0.262866f, 0.951056f}, {-0.681718f, 0.147621f, 0.716567f}, {-0.809017f, 0.309017f, 0.500000f}, {-0.587785f, 0.425325f, 0.688191f}, {-0.850651f, 0.525731f, 0.000000f}, {-0.864188f, 0.442863f, 0.238856f}, {-0.716567f, 0.681718f, 0.147621f}, {-0.688191f, 0.587785f, 0.425325f}, {-0.500000f, 0.809017f, 0.309017f}, {-0.238856f, 0.864188f, 0.442863f}, {-0.425325f, 0.688191f, 0.587785f}, {-0.716567f, 0.681718f, -0.147621f}, {-0.500000f, 0.809017f, -0.309017f}, {-0.525731f, 0.850651f, 0.000000f}, {0.000000f, 0.850651f, -0.525731f}, {-0.238856f, 0.864188f, -0.442863f}, {0.000000f, 0.955423f, -0.295242f}, {-0.262866f, 0.951056f, -0.162460f}, {0.000000f, 1.000000f, 0.000000f}, {0.000000f, 0.955423f, 0.295242f}, {-0.262866f, 0.951056f, 0.162460f}, {0.238856f, 0.864188f, 0.442863f}, {0.262866f, 0.951056f, 0.162460f}, {0.500000f, 0.809017f, 0.309017f}, {0.238856f, 0.864188f, -0.442863f}, {0.262866f, 0.951056f, -0.162460f}, {0.500000f, 0.809017f, -0.309017f}, {0.850651f, 0.525731f, 0.000000f}, {0.716567f, 0.681718f, 0.147621f}, {0.716567f, 0.681718f, -0.147621f}, {0.525731f, 0.850651f, 0.000000f}, {0.425325f, 0.688191f, 0.587785f}, {0.864188f, 0.442863f, 0.238856f}, {0.688191f, 0.587785f, 0.425325f}, {0.809017f, 0.309017f, 0.500000f}, {0.681718f, 0.147621f, 0.716567f}, {0.587785f, 0.425325f, 0.688191f}, {0.955423f, 0.295242f, 0.000000f}, {1.000000f, 0.000000f, 0.000000f}, {0.951056f, 0.162460f, 0.262866f}, {0.850651f, -0.525731f, 0.000000f}, {0.955423f, -0.295242f, 0.000000f}, {0.864188f, -0.442863f, 0.238856f}, {0.951056f, -0.162460f, 0.262866f}, {0.809017f, -0.309017f, 0.500000f}, {0.681718f, -0.147621f, 0.716567f}, {0.850651f, 0.000000f, 0.525731f}, {0.864188f, 0.442863f, -0.238856f}, {0.809017f, 0.309017f, -0.500000f}, {0.951056f, 0.162460f, -0.262866f}, {0.525731f, 0.000000f, -0.850651f}, {0.681718f, 0.147621f, -0.716567f}, {0.681718f, -0.147621f, -0.716567f}, {0.850651f, 0.000000f, -0.525731f}, {0.809017f, -0.309017f, -0.500000f}, {0.864188f, -0.442863f, -0.238856f}, {0.951056f, -0.162460f, -0.262866f}, {0.147621f, 0.716567f, -0.681718f}, {0.309017f, 0.500000f, -0.809017f}, {0.425325f, 0.688191f, -0.587785f}, {0.442863f, 0.238856f, -0.864188f}, {0.587785f, 0.425325f, -0.688191f}, {0.688191f, 0.587785f, -0.425325f}, {-0.147621f, 0.716567f, -0.681718f}, {-0.309017f, 0.500000f, -0.809017f}, {0.000000f, 0.525731f, -0.850651f}, {-0.525731f, 0.000000f, -0.850651f}, {-0.442863f, 0.238856f, -0.864188f}, {-0.295242f, 0.000000f, -0.955423f}, {-0.162460f, 0.262866f, -0.951056f}, {0.000000f, 0.000000f, -1.000000f}, {0.295242f, 0.000000f, -0.955423f}, {0.162460f, 0.262866f, -0.951056f}, {-0.442863f, -0.238856f, -0.864188f}, {-0.309017f, -0.500000f, -0.809017f}, {-0.162460f, -0.262866f, -0.951056f}, {0.000000f, -0.850651f, -0.525731f}, {-0.147621f, -0.716567f, -0.681718f}, {0.147621f, -0.716567f, -0.681718f}, {0.000000f, -0.525731f, -0.850651f}, {0.309017f, -0.500000f, -0.809017f}, {0.442863f, -0.238856f, -0.864188f}, {0.162460f, -0.262866f, -0.951056f}, {0.238856f, -0.864188f, -0.442863f}, {0.500000f, -0.809017f, -0.309017f}, {0.425325f, -0.688191f, -0.587785f}, {0.716567f, -0.681718f, -0.147621f}, {0.688191f, -0.587785f, -0.425325f}, {0.587785f, -0.425325f, -0.688191f}, {0.000000f, -0.955423f, -0.295242f}, {0.000000f, -1.000000f, 0.000000f}, {0.262866f, -0.951056f, -0.162460f}, {0.000000f, -0.850651f, 0.525731f}, {0.000000f, -0.955423f, 0.295242f}, {0.238856f, -0.864188f, 0.442863f}, {0.262866f, -0.951056f, 0.162460f}, {0.500000f, -0.809017f, 0.309017f}, {0.716567f, -0.681718f, 0.147621f}, {0.525731f, -0.850651f, 0.000000f}, {-0.238856f, -0.864188f, -0.442863f}, {-0.500000f, -0.809017f, -0.309017f}, {-0.262866f, -0.951056f, -0.162460f}, {-0.850651f, -0.525731f, 0.000000f}, {-0.716567f, -0.681718f, -0.147621f}, {-0.716567f, -0.681718f, 0.147621f}, {-0.525731f, -0.850651f, 0.000000f}, {-0.500000f, -0.809017f, 0.309017f}, {-0.238856f, -0.864188f, 0.442863f}, {-0.262866f, -0.951056f, 0.162460f}, {-0.864188f, -0.442863f, 0.238856f}, {-0.809017f, -0.309017f, 0.500000f}, {-0.688191f, -0.587785f, 0.425325f}, {-0.681718f, -0.147621f, 0.716567f}, {-0.442863f, -0.238856f, 0.864188f}, {-0.587785f, -0.425325f, 0.688191f}, {-0.309017f, -0.500000f, 0.809017f}, {-0.147621f, -0.716567f, 0.681718f}, {-0.425325f, -0.688191f, 0.587785f}, {-0.162460f, -0.262866f, 0.951056f}, {0.442863f, -0.238856f, 0.864188f}, {0.162460f, -0.262866f, 0.951056f}, {0.309017f, -0.500000f, 0.809017f}, {0.147621f, -0.716567f, 0.681718f}, {0.000000f, -0.525731f, 0.850651f}, {0.425325f, -0.688191f, 0.587785f}, {0.587785f, -0.425325f, 0.688191f}, {0.688191f, -0.587785f, 0.425325f}, {-0.955423f, 0.295242f, 0.000000f}, {-0.951056f, 0.162460f, 0.262866f}, {-1.000000f, 0.000000f, 0.000000f}, {-0.850651f, 0.000000f, 0.525731f}, {-0.955423f, -0.295242f, 0.000000f}, {-0.951056f, -0.162460f, 0.262866f}, {-0.864188f, 0.442863f, -0.238856f}, {-0.951056f, 0.162460f, -0.262866f}, {-0.809017f, 0.309017f, -0.500000f}, {-0.864188f, -0.442863f, -0.238856f}, {-0.951056f, -0.162460f, -0.262866f}, {-0.809017f, -0.309017f, -0.500000f}, {-0.681718f, 0.147621f, -0.716567f}, {-0.681718f, -0.147621f, -0.716567f}, {-0.850651f, 0.000000f, -0.525731f}, {-0.688191f, 0.587785f, -0.425325f}, {-0.587785f, 0.425325f, -0.688191f}, {-0.425325f, 0.688191f, -0.587785f}, {-0.425325f, -0.688191f, -0.587785f}, {-0.587785f, -0.425325f, -0.688191f}, {-0.688191f, -0.587785f, -0.425325f}};

int Q_rand(int *seed) {
	*seed = (69069 * *seed + 1);
	return *seed;
}

float Q_random(int *seed) { return (Q_rand(seed) & 0xffff) / (float)0x10000; }

float Q_crandom(int *seed) { return 2.0 * (Q_random(seed) - 0.5); }

int DirToByte(vec3_t dir) {
	int i, best;
	float d, bestd;

	if(!dir) return 0;

	bestd = 0;
	best = 0;
	for(i = 0; i < NUMVERTEXNORMALS; i++) {
		d = DotProduct(dir, bytedirs[i]);
		if(d > bestd) {
			bestd = d;
			best = i;
		}
	}

	return best;
}

void ByteToDir(int b, vec3_t dir) {
	if(b < 0 || b >= NUMVERTEXNORMALS) {
		VectorCopy(vec3_origin, dir);
		return;
	}
	VectorCopy(bytedirs[b], dir);
}

void RotatePointAroundVector(vec3_t dst, const vec3_t dir, const vec3_t point, float degrees) {
	float m[3][3];
	float im[3][3];
	float zrot[3][3];
	float tmpmat[3][3];
	float rot[3][3];
	int i;
	vec3_t vr, vup, vf;
	float rad;

	vf[0] = dir[0];
	vf[1] = dir[1];
	vf[2] = dir[2];

	PerpendicularVector(vr, dir);
	CrossProduct(vr, vf, vup);

	m[0][0] = vr[0];
	m[1][0] = vr[1];
	m[2][0] = vr[2];

	m[0][1] = vup[0];
	m[1][1] = vup[1];
	m[2][1] = vup[2];

	m[0][2] = vf[0];
	m[1][2] = vf[1];
	m[2][2] = vf[2];

	memcpy(im, m, sizeof(im));

	im[0][1] = m[1][0];
	im[0][2] = m[2][0];
	im[1][0] = m[0][1];
	im[1][2] = m[2][1];
	im[2][0] = m[0][2];
	im[2][1] = m[1][2];

	memset(zrot, 0, sizeof(zrot));
	zrot[0][0] = zrot[1][1] = zrot[2][2] = 1.0F;

	rad = DEG2RAD(degrees);
	zrot[0][0] = cos(rad);
	zrot[0][1] = sin(rad);
	zrot[1][0] = -sin(rad);
	zrot[1][1] = cos(rad);

	MatrixMultiply(m, zrot, tmpmat);
	MatrixMultiply(tmpmat, im, rot);

	for(i = 0; i < 3; i++) {
		dst[i] = rot[i][0] * point[0] + rot[i][1] * point[1] + rot[i][2] * point[2];
	}
}

void RotateAroundDirection(vec3_t axis[3], float yaw) {
	PerpendicularVector(axis[1], axis[0]);

	if(yaw) {
		vec3_t temp;

		VectorCopy(axis[1], temp);
		RotatePointAroundVector(axis[1], axis[0], temp, yaw);
	}
	
	CrossProduct(axis[0], axis[1], axis[2]);
}

void vectoangles(const vec3_t value1, vec3_t angles) {
	float forward;
	float yaw, pitch;

	if(value1[1] == 0 && value1[0] == 0) {
		yaw = 0;
		if(value1[2] > 0) pitch = 90;
		else pitch = 270;
	} else {
		if(value1[0]) yaw = (atan2(value1[1], value1[0]) * 180 / M_PI);
		else if(value1[1] > 0) yaw = 90;
		else yaw = 270;
		if(yaw < 0) yaw += 360;

		forward = sqrt(value1[0] * value1[0] + value1[1] * value1[1]);
		pitch = (atan2(value1[2], forward) * 180 / M_PI);
		if(pitch < 0) pitch += 360;
	}

	angles[PITCH] = -pitch;
	angles[YAW] = yaw;
	angles[ROLL] = 0;
}

void AnglesToAxis(const vec3_t angles, vec3_t axis[3]) {
	vec3_t right;

	AngleVectors(angles, axis[0], right, axis[2]);
	VectorSubtract(vec3_origin, right, axis[1]);
}

void AxisToAngles(vec3_t axis[3], vec3_t angles) {
    angles[0] = atan2(-axis[2][0], sqrt(axis[2][1] * axis[2][1] + axis[2][2] * axis[2][2])) * (180 / M_PI);
	angles[1] = atan2(axis[1][0], axis[0][0]) * (180 / M_PI);
	angles[2] = atan2(axis[2][1], axis[2][2]) * (180 / M_PI);
}

void OrthogonalizeMatrix(vec3_t forward, vec3_t right, vec3_t up) {
	vec3_t testUp;
	float dot;
	VectorNormalize(forward);

	dot = DotProduct(right, forward);
	VectorMA(right, -dot, forward, right);
	VectorNormalize(right);

	CrossProduct(forward, right, up);

	CrossProduct(forward, right, testUp);
	if(DotProduct(testUp, up) < 0.0f) VectorNegate(up, up);
}

void VelocityToAxis(const vec3_t velocity, vec3_t axis[3], float lerpFactor) {
	vec3_t up, targetForward, targetRight, targetUp;
	int i;

	if(VectorLength(velocity) == 0) return;

	VectorNormalize2(velocity, targetForward);

	VectorSet(up, 0, 0, 1);

	CrossProduct(up, targetForward, targetRight);
	VectorNormalize(targetRight);

	CrossProduct(targetForward, targetRight, targetUp);

	for(i = 0; i < 3; i++) {
		axis[0][i] = Lerp(axis[0][i], targetForward[i], lerpFactor); // forward
		axis[1][i] = Lerp(axis[1][i], targetRight[i], lerpFactor);   // right
		axis[2][i] = Lerp(axis[2][i], targetUp[i], lerpFactor);      // up
	}

	VectorNormalize(axis[0]);
	CrossProduct(axis[2], axis[0], axis[1]);
	VectorNormalize(axis[1]);
	CrossProduct(axis[0], axis[1], axis[2]);
	VectorNormalize(axis[2]);
}

float Lerp(float a, float b, float f) { return a + f * (b - a); }

void AxisClear(vec3_t axis[3]) {
	axis[0][0] = 1;
	axis[0][1] = 0;
	axis[0][2] = 0;
	axis[1][0] = 0;
	axis[1][1] = 1;
	axis[1][2] = 0;
	axis[2][0] = 0;
	axis[2][1] = 0;
	axis[2][2] = 1;
}

void AxisCopy(vec3_t in[3], vec3_t out[3]) {
	VectorCopy(in[0], out[0]);
	VectorCopy(in[1], out[1]);
	VectorCopy(in[2], out[2]);
}

void ProjectPointOnPlane(vec3_t dst, const vec3_t p, const vec3_t normal) {
	float d;
	vec3_t n;
	float inv_denom;

	inv_denom = DotProduct(normal, normal);
	inv_denom = 1.0f / inv_denom;

	d = DotProduct(normal, p) * inv_denom;

	n[0] = normal[0] * inv_denom;
	n[1] = normal[1] * inv_denom;
	n[2] = normal[2] * inv_denom;

	dst[0] = p[0] - d * n[0];
	dst[1] = p[1] - d * n[1];
	dst[2] = p[2] - d * n[2];
}

float LerpAngle(float from, float to, float frac) {
	float a;

	if(to - from > 180) to -= 360;
	if(to - from < -180) to += 360;
	a = from + frac * (to - from);

	return a;
}

float AngleSubtract(float a1, float a2) {
	float a;

	a = a1 - a2;
	while(a > 180) a -= 360;
	while(a < -180) a += 360;
	return a;
}

void AnglesSubtract(vec3_t v1, vec3_t v2, vec3_t v3) {
	v3[0] = AngleSubtract(v1[0], v2[0]);
	v3[1] = AngleSubtract(v1[1], v2[1]);
	v3[2] = AngleSubtract(v1[2], v2[2]);
}

float AngleMod(float a) {
	a = (360.0 / 65536) * ((int)(a * (65536 / 360.0)) & 65535);
	return a;
}

float AngleAdd(float a1, float a2) {
	float a;

	a = a1 + a2;
	while(a > 180.0) a -= 360.0;
	while(a < -180.0) a += 360.0;
	return a;
}

void AngleMA(vec3_t aa, float scale, vec3_t ab, vec3_t ac) {
	ac[0] = AngleAdd(aa[0], scale * ab[0]);
	ac[1] = AngleAdd(aa[1], scale * ab[1]);
	ac[2] = AngleAdd(aa[2], scale * ab[2]);
}

void LerpAngles(vec3_t from, vec3_t to, vec3_t dest, float frac) {
	dest[0] = LerpAngle(from[0], to[0], frac);
	dest[1] = LerpAngle(from[1], to[1], frac);
	dest[2] = LerpAngle(from[2], to[2], frac);
}

float AngleNormalize360(float angle) { return (360.0 / 65536) * ((int)(angle * (65536 / 360.0)) & 65535); }

float AngleNormalize180(float angle) {
	angle = AngleNormalize360(angle);
	if(angle > 180.0) angle -= 360.0;
	return angle;
}

float AngleDelta(float angle1, float angle2) { return AngleNormalize180(angle1 - angle2); }

float RadiusFromBounds(const vec3_t mins, const vec3_t maxs) {
	int i;
	vec3_t corner;
	float a, b;

	for(i = 0; i < 3; i++) {
		a = fabs(mins[i]);
		b = fabs(maxs[i]);
		corner[i] = a > b ? a : b;
	}

	return VectorLength(corner);
}

void AddPointToBounds(const vec3_t v, vec3_t mins, vec3_t maxs) {
	if(v[0] < mins[0]) mins[0] = v[0];
	if(v[0] > maxs[0]) maxs[0] = v[0];
	if(v[1] < mins[1]) mins[1] = v[1];
	if(v[1] > maxs[1]) maxs[1] = v[1];
	if(v[2] < mins[2]) mins[2] = v[2];
	if(v[2] > maxs[2]) maxs[2] = v[2];
}

vec_t VectorNormalize(vec3_t v) {
	float length, ilength;

	length = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	length = sqrt(length);

	if(length) {
		ilength = 1 / length;
		v[0] *= ilength;
		v[1] *= ilength;
		v[2] *= ilength;
	}

	return length;
}

vec_t VectorNormalize2(const vec3_t v, vec3_t out) {
	float length, ilength;

	length = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	length = sqrt(length);

	if(length) {
		ilength = 1 / length;
		out[0] = v[0] * ilength;
		out[1] = v[1] * ilength;
		out[2] = v[2] * ilength;
	} else {
		VectorClear(out);
	}

	return length;
}

void MatrixMultiply(float in1[3][3], float in2[3][3], float out[3][3]) {
	out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] + in1[0][2] * in2[2][0];
	out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] + in1[0][2] * in2[2][1];
	out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] + in1[0][2] * in2[2][2];
	out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] + in1[1][2] * in2[2][0];
	out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] + in1[1][2] * in2[2][1];
	out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] + in1[1][2] * in2[2][2];
	out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] + in1[2][2] * in2[2][0];
	out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] + in1[2][2] * in2[2][1];
	out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] + in1[2][2] * in2[2][2];
}

void AngleVectors(const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up) {
	float angle;
	static float sr, sp, sy, cr, cp, cy;
	// static to help MS compiler fp bugs

	angle = angles[YAW] * (M_PI * 2 / 360);
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[PITCH] * (M_PI * 2 / 360);
	sp = sin(angle);
	cp = cos(angle);
	angle = angles[ROLL] * (M_PI * 2 / 360);
	sr = sin(angle);
	cr = cos(angle);

	if(forward) {
		forward[0] = cp * cy;
		forward[1] = cp * sy;
		forward[2] = -sp;
	}
	if(right) {
		right[0] = (-1 * sr * sp * cy + -1 * cr * -sy);
		right[1] = (-1 * sr * sp * sy + -1 * cr * cy);
		right[2] = -1 * sr * cp;
	}
	if(up) {
		up[0] = (cr * sp * cy + -sr * -sy);
		up[1] = (cr * sp * sy + -sr * cy);
		up[2] = cr * cp;
	}
}

void PerpendicularVector(vec3_t dst, const vec3_t src) {
	int pos;
	int i;
	float minelem = 1.0F;
	vec3_t tempvec;

	for(pos = 0, i = 0; i < 3; i++) {
		if(fabs(src[i]) < minelem) {
			pos = i;
			minelem = fabs(src[i]);
		}
	}
	tempvec[0] = tempvec[1] = tempvec[2] = 0.0F;
	tempvec[pos] = 1.0F;

	ProjectPointOnPlane(dst, tempvec, src);

	VectorNormalize(dst);
}

void SnapVectorTowards(vec3_t v, vec3_t to) {
	int i;

	for(i = 0; i < 3; i++) {
		if(to[i] <= v[i]) v[i] = (int)v[i];
		else v[i] = (int)v[i] + 1;
	}
}

int VectorCompare(const vec3_t v1, const vec3_t v2) {
	if(v1[0] != v2[0] || v1[1] != v2[1] || v1[2] != v2[2]) return 0;
	return 1;
}

vec_t VectorLength(const vec3_t v) { return (vec_t)sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]); }

vec_t VectorLengthSquared(const vec3_t v) { return (v[0] * v[0] + v[1] * v[1] + v[2] * v[2]); }

float VectorDistance(const vec3_t v1, const vec3_t v2) {
	vec3_t v;

	VectorSubtract(v2, v1, v);
	return VectorLength(v);
}

void VectorInverse(vec3_t v) {
	v[0] = -v[0];
	v[1] = -v[1];
	v[2] = -v[2];
}

void CrossProduct(const vec3_t v1, const vec3_t v2, vec3_t cross) {
	cross[0] = v1[1] * v2[2] - v1[2] * v2[1];
	cross[1] = v1[2] * v2[0] - v1[0] * v2[2];
	cross[2] = v1[0] * v2[1] - v1[1] * v2[0];
}
