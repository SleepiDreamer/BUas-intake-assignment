// Template, BUAS version https://www.buas.nl/games
// IGAD/BUAS(NHTV)/UU - Jacco Bikker - 2006-2020

#pragma once
#pragma warning disable warning-list
#include "template.h"

namespace Tmpl8 {

constexpr int RedMask = 0xff0000;
constexpr int GreenMask = 0x00ff00;
constexpr int BlueMask = 0x0000ff;

typedef unsigned int Pixel; // unsigned int is assumed to be 32-bit, which seems a safe assumption.

inline Pixel AddBlend( Pixel a_Color1, Pixel a_Color2 )
{
	const unsigned int r = (a_Color1 & RedMask) + (a_Color2 & RedMask);
	const unsigned int g = (a_Color1 & GreenMask) + (a_Color2 & GreenMask);
	const unsigned int b = (a_Color1 & BlueMask) + (a_Color2 & BlueMask);
	const unsigned r1 = (r & RedMask) | (RedMask * (r >> 24));
	const unsigned g1 = (g & GreenMask) | (GreenMask * (g >> 16));
	const unsigned b1 = (b & BlueMask) | (BlueMask * (b >> 8));
	return (r1 + g1 + b1);
}

// subtractive blending
inline Pixel SubBlend( Pixel a_Color1, Pixel a_Color2 )
{
	int red = (a_Color1 & RedMask) - (a_Color2 & RedMask);
	int green = (a_Color1 & GreenMask) - (a_Color2 & GreenMask);
	int blue = (a_Color1 & BlueMask) - (a_Color2 & BlueMask);
	if (red < 0) red = 0;
	if (green < 0) green = 0;
	if (blue < 0) blue = 0;
	return static_cast<Pixel>(red + green + blue);
}

Pixel AlphaBlend(int color1, int color2, float alpha);

class Surface
{
	enum { OWNER = 1 };
public:
	// constructor / destructor
	Surface( int a_Width, int a_Height, Pixel* a_Buffer, int a_Pitch );
	Surface( int a_Width, int a_Height );
	Surface( const char* a_File );
	~Surface();
	// member data access
	Pixel* GetBuffer() const { return m_Buffer; }
	void SetBuffer( Pixel* a_Buffer ) { m_Buffer = a_Buffer; }
	int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }
	int GetPitch() const { return m_Pitch; }
	void SetPitch( int a_Pitch ) { m_Pitch = a_Pitch; }
	// Special operations
	void InitCharset();
	void SetChar( int c, char* c1, char* c2, char* c3, char* c4, char* c5 );
	void Print( const char* a_String, int x1, int y1, Pixel color );
	void PrintScaled(const char* a_String, int x1, int y1, int xscale, int yscale, Pixel color);
	void PrintCentre( const char* a_String, int y1, Pixel color );
	void PrintCentreScaled(const char* a_String, int y1, int xscale, int yscale, Pixel color);
	void Clear( Pixel a_Color ) const;
	void Line( float x1, float y1, float x2, float y2, Pixel color ) const;
	void LineClipped(vec2 _pos1, vec2 _pos2, vec4 _window, Pixel c) const;
	void Plot( int x, int y, Pixel c, float _alpha = 1.0f ) const;
	void LoadImage( const char* a_File );
	void CopyTo( Surface* a_Dst, int a_X, int a_Y ) const;
	void BlendCopyTo( Surface* a_Dst, int a_X, int a_Y ) const;
	void ScaleColor( unsigned int a_Scale ) const;
	void Box( int x1, int y1, int x2, int y2, Pixel color ) const;
	void Box( vec2 pos1, vec2 pos2, Pixel color ) const;
	void BoxThick(int x1, int y1, int x2, int y2, int width, Pixel c) const;
	void BoxThick(vec2 pos1, vec2 pos2, int width, Pixel c) const;
	void Bar( int x1, int y1, int x2, int y2, Pixel color, float alpha = 1.0f ) const;
	void Bar(vec2 pos1, vec2 pos2, Pixel color, float alpha = 1.0f ) const;
	void BarShadow(vec2 _pos1, vec2 _pos2, float _r, float _alpha) const;
	void BarCentre(int y1, int y2, int width, Pixel c) const;
	void Circle(vec2 _pos, int _r, Pixel _c) const;
	void CircleFull(vec2 _pos, float _rMin, float _rMax, Pixel _c, float _alpha = 1.0f) const;
	void CircleShadow(vec2 _pos, float _r, float _alpha) const;
	void Vignette(float _strength) const;
	void DrawView(int x1, int y1, int x2, int y2, Pixel color) const; // OLD
	bool CheckVisibility(float x1, float y1, float x2, float y2, float bx1, float by1, float bx2, float by2); // OLD
	bool CheckFullVisibility(float x1, float y1, float x2, float y2, float bx1, float by1, float bx2, float by2); // OLD
	int Visibility(float x1, float y1, float x2, float y2, float bx1, float by1, float bx2, float by2); // OLD
	void Resize( Surface* a_Orig ) const;
private:
	// Attributes
	Pixel* m_Buffer{nullptr};	
	int m_Width{0}, m_Height{0};
	int m_Pitch{0};
	int m_Flags{0};
	// Static attributes for the buildin font
	static char s_Font[51][5][6];
	static bool fontInitialized;
	int s_Transl[256]{};		
};

class Sprite
{
public:
	// Sprite flags
	enum
	{
		FLARE		= (1<< 0),
		OPFLARE		= (1<< 1),	
		FLASH		= (1<< 4),	
		DISABLED	= (1<< 6),	
		GMUL		= (1<< 7),
		BLACKFLARE	= (1<< 8),	
		BRIGHTEST   = (1<< 9),
		RFLARE		= (1<<12),
		GFLARE		= (1<<13),
		NOCLIP		= (1<<14)
	};
	
	// Structors
	Sprite( Surface* a_Surface, unsigned int a_NumFrames );
	~Sprite();
	// Methods
	void Draw( Surface* a_Target, int a_X, int a_Y );
	void DrawScaled(int a_X, int a_Y, int a_Width, int a_Height, Surface* a_Target, bool is_flipped = false);
	void SetFlags( unsigned int a_Flags ) { m_Flags = a_Flags; }
	void SetFrame( unsigned int a_Index ) { m_CurrentFrame = a_Index; }
	unsigned int GetFlags() const { return m_Flags; }
	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }
	Pixel* GetBuffer() { return m_Surface->GetBuffer(); }	
	unsigned int Frames() { return m_NumFrames; }
	Surface* GetSurface() { return m_Surface; }
private:
	// Methods
	void InitializeStartData();
	// Attributes
	int m_Width, m_Height, m_Pitch;
	unsigned int m_NumFrames;          
	unsigned int m_CurrentFrame;       
	unsigned int m_Flags;
	unsigned int** m_Start;
	Surface* m_Surface;
};

class Font
{
public:
	Font();
	Font( char* a_File, char* a_Chars );
	~Font();
	void Print( Surface* a_Target, char* a_Text, int a_X, int a_Y, bool clip = false ) const;
	void Centre( Surface* a_Target, char* a_Text, int a_Y );
	int Width( char* a_Text ) const;
	int Height() { return m_Surface->GetHeight(); }
	void YClip( int y1, int y2 ) { m_CY1 = y1; m_CY2 = y2; }
private:
	Surface* m_Surface;
	int* m_Offset, *m_Width, *m_Trans, m_Height, m_CY1, m_CY2;
};

}; // namespace Tmpl8