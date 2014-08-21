//
//  
// 
//
//  Created by Paul Rudolph on 8/8/13.
//
//

#pragma once

#include "cinder/app/AppBasic.h"
//#include "cinder/Text.h"
#include "cinder/Font.h"
#include "cinder/gl/Texture.h"
#include "cinder/Timeline.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Json.h"

namespace bci{
	

	enum TextAlignment	{ AlignmentUnset, Left, Center, Right };
	
	//References
	typedef std::shared_ptr<class		TextObject>		TextObjectRef;
	typedef std::shared_ptr<const class TextObject>		TextObjectConstRef;
	typedef std::weak_ptr  <class		TextObject>		TextObjectWeakRef;


	//Styling
	struct Style{
		std::string		styleName;
		std::string		fontName;
		float			size;
		float			leading;

		TextAlignment	alignment;// LEFT CENTER RIGHT
		cinder::Color	color;//COLOR

		bool fontNameSet;
		bool sizeSet;
		bool leadingSet;
		bool styleSet;
		bool alignmentSet;
		bool colorSet;
		Style() :styleName(""),
			fontName(""),
			size(0.0f),
			leading(0.0f),
			alignment(AlignmentUnset),
			color(-1, -1, -1),
			fontNameSet(false),
			leadingSet(0.0f),
			sizeSet(false),
			alignmentSet(false),
			colorSet(false){};
	};
	typedef std::shared_ptr<struct Style>	StyleRef;
	typedef std::map<std::string, StyleRef> StyleElements;


	
	class TextObject {
	
		public:
		
		
		TextObject();
		~TextObject();
		static  void	loadStyles(const std::string &styleFilePath);

		static  TextObjectRef	create(const cinder::Vec2f &pos, const cinder::Vec2i &size, const std::string &textString);

		//Public variables to be used for animations
		cinder::Anim<float>			alpha;
		cinder::Anim<cinder::Vec2f>	offsetAnim;
		
		cinder::Vec2f getPosition(){ return mPosition + offsetAnim; };


		cinder::gl::TextureRef getTexture(); 

		void renderTextBox(std::string forcedStyle = "");
		void renderTextLayout();
		void renderTextureFont(bool wordWrap =true,std::string forcedStyle = "");
		void drawTextureFont(bool wordWrap = true, std::string forcedStyle = "");

		static cinder::Font getFont(std::string fontName, float fontSize);
		static void clearFontCache(){ FontCache.clear(); };
		std::string getTextString(){ return mTextString; };
		void setPosition(const cinder::Vec2f &pos){ mPosition = pos; };
		void setString(std::string s){ mTextString = s; };
	private:
		void setupFbo();
		void destroyFbo();
		std::vector<std::string> parseText();
		static void setTextAlignment(StyleRef style, const cinder::JsonTree &styleElememtsDoc);
		static void setFontName(StyleRef style, const cinder::JsonTree &styleElememtsDoc);
		static void setFontSize(StyleRef style, const cinder::JsonTree &styleElememtsDoc);
		static void setFontColor(StyleRef style, const cinder::JsonTree &styleElememtsDoc);
		static void setLeading(StyleRef style, const cinder::JsonTree &styleElememtsDoc);

		void updateTextLayoutStyle(StyleRef style, cinder::TextLayout &textLayout, const std::vector<StyleRef> &styleStack);
		void setTextBoxStyle(StyleRef style, cinder::TextBox &textBox);


		cinder::gl::TextureRef						mTexture;
		cinder::Vec2f								mPosition;
		std::string									mTextString;
		cinder::gl::Fbo								mTextFbo;
		int										mWidth, mHeight;

		static StyleElements styleElements;


		static  std::map<std::string, cinder::DataSourceRef> FontCache;


	};
};