//
//  TextUtils.cpp
// 
//
//  Created by Paul Rudolph on 8/8/13.
//
//

#include "TextObject.h"
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/reversed.hpp>

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace boost;

namespace bci{

	StyleElements TextObject ::styleElements;
	std::map<std::string, cinder::DataSourceRef> TextObject::FontCache;

	TextObject::TextObject():
		mPosition(cinder::Vec2f::zero()),
		offsetAnim(cinder::Vec2f::zero()),
		alpha(1.0f),
		mTextString(""),
		mWidth(0),
		mHeight(0)
	{

	
	}

	TextObject::~TextObject(){}
	//STYLES//////////////////////////////////////////////
	void TextObject::loadStyles(const std::string &styleFilePath){

		//load file
		JsonTree doc(loadAsset(styleFilePath));
		

		//THIS WILL BE THE DEFAULT STYLE
		StyleRef styleObj = StyleRef(new Style());
		styleObj->alignmentSet = true;
		styleObj->alignment = Left;

		styleObj->colorSet = true;
		styleObj->color = Color(1, 1, 1);

		styleObj->fontNameSet = true;
		styleObj->fontName = "Arial";

		styleObj->leadingSet = true;
		styleObj->leading = 10.0f;
			
		styleObj->sizeSet = true;
		styleObj->size = 15.0f;

		styleObj->styleSet = true;
		styleObj->styleName = "DEFAULT";
		styleElements[styleObj->styleName] = styleObj;
		////////////////////////////////




		for (auto styleDoc : doc.getChild("Styles")){
				StyleRef styleObj = StyleRef(new Style());
				string styleName = styleDoc.getKey();
				styleObj->styleName = styleName;
			
				setTextAlignment(styleObj, styleDoc);
				setFontName(styleObj, styleDoc);
				setFontSize(styleObj, styleDoc);
				setFontColor(styleObj, styleDoc);
				setLeading(styleObj, styleDoc);
				styleElements[styleName] = styleObj;
			}

			console() << "Styles loaded" << endl;
	
	}
	
	void TextObject::setFontName(StyleRef style, const cinder::JsonTree &styleElememtsDoc){
		if (styleElememtsDoc.hasChild("font") ){
			std::string fontnameString = styleElememtsDoc.getChild("font").getValue();
	

         
            
			if (std::find(Font::getNames().begin(), Font::getNames().end(), fontnameString) != Font::getNames().end()){
				style->fontName = fontnameString;
				style->fontNameSet = true;
			}
			else{
                fs::path fontPath = getAssetPath("Fonts/" + fontnameString + ".ttf");
				if (fs::exists(fontPath)){
					style->fontName = fontnameString;
					style->fontNameSet = true;
				}
				else{
					console() << "FONT STYLE WARNING: Could Not find font: " << fontnameString << " ,Using Default font : \"Arial\" size:10" << endl;
					console() << "\t\t\t - Make Sure .ttf File exists in \"Assets/Fonts/...." << endl;
				}
			}
		}
	}

	void TextObject::setFontSize(StyleRef style, const cinder::JsonTree &styleElememtsDoc){
		if (styleElememtsDoc.hasChild("size")){
			std::string fontSizeString = styleElememtsDoc.getChild("size").getValue();

			try{
				style->size = std::stof(fontSizeString);
				style->sizeSet = true;
			}
			catch (...){
				console() << "Malformed Font Size: \"" << fontSizeString << "\"" << endl;
			}
		}
	}

	void TextObject::setFontColor(StyleRef style, const cinder::JsonTree &styleElememtsDoc){
		//Set Default Color
		float red	= 0.0f;
		float green = 0.0f;
		float blue	= 0.0f;

		std::string colorString = styleElememtsDoc.getChild("color").getValue();

		try{
			red		= (float)std::stoul(colorString.substr(1, 2), nullptr, 16);
			green	= (float)std::stoul(colorString.substr(3, 2), nullptr, 16);
			blue	= (float)std::stoul(colorString.substr(5, 2), nullptr, 16);
		}
		catch (...){
			console() << "FONT STYLE WARNING: Conversion for font color : \"" << colorString << "\" Not Successful - Default Loaded" << endl;

		}
		style->color = Color(red / 255.0f, green / 255.0f, blue / 255.0f);
		style->colorSet = true;

	}

	void TextObject::setTextAlignment(StyleRef style, const JsonTree &styleElememtsDoc){
		if (styleElememtsDoc.hasChild("alignment")){

			std::string alignmentString = styleElememtsDoc.getChild("alignment").getValue();
			std::string sanitizedAlignmentString = to_lower_copy(alignmentString);

			TextAlignment textAlignment = Left;//Left Will be default

			if (sanitizedAlignmentString == "left")	textAlignment = Left;
			else if (sanitizedAlignmentString == "center")	textAlignment = Center;
			else if (sanitizedAlignmentString == "right")	textAlignment = Right;
			else    console() << "FONT STYLE WARNING: Conversion for Text alignment : \"" << alignmentString << "\" Not Successful - Default Loaded" << endl;

			style->alignment = textAlignment;
			style->alignmentSet = true;
		}
	}

	void TextObject::setLeading(StyleRef style, const cinder::JsonTree &styleElememtsDoc){
		if (styleElememtsDoc.hasChild("leading")){
			std::string leadingString = styleElememtsDoc.getChild("leading").getValue();

			try{
				style->leading = std::stof(leadingString);
				style->leadingSet = true;
			}
			catch (...){
				console() << "Malformed Font Size: \"" << leadingString << "\"" << endl;
			}
		}
	}

	////////////////////////////////////////////////


	TextObjectRef	TextObject::create(const cinder::Vec2f &pos, const cinder::Vec2i &size, const string &text){
		if (styleElements.empty()) TextObject::loadStyles("TextStyles.json");

		TextObjectRef textObjectRef(new TextObject());
		
		textObjectRef->mPosition		= pos;
		textObjectRef->mWidth			= size.x;
		textObjectRef->mHeight			= size.y;
		textObjectRef->mTextString		= text;
	
		return textObjectRef;
	}

	/**
	* This breaks up the text based on styling tags and returns the tokens
	*/
	std::vector<std::string>TextObject::parseText(){
		vector<string> tokens;
		size_t openTagPos = 0, openTagStartSearchPos = 0;
		while ((openTagPos = mTextString.find_first_of("<", openTagStartSearchPos)) != string::npos)
		{
			//Get the text that is inbetween the metatags
			
			if (openTagStartSearchPos > 0){
				std::string tokenString = mTextString.substr(openTagStartSearchPos, openTagPos - openTagStartSearchPos);
				
				if (tokenString.size()>0)tokens.push_back(tokenString);
			}
			
			size_t closeTagPos = 0;

			if ((closeTagPos = mTextString.find_first_of(">", openTagPos + 1)) != string::npos){

				std::string tokenString = mTextString.substr(openTagPos, closeTagPos - openTagPos + 1);
				tokens.push_back(tokenString);
                openTagStartSearchPos = closeTagPos + 1;
			}
			else{
				console() << "FONT STYLE WARNING: Malformed Style Tag : @" << mTextString.substr(openTagPos, openTagPos - mTextString.length()  ) << endl;
				break;
			}
		}

		if ( tokens.empty() && mTextString.length() > 0){
			tokens.push_back(mTextString);
		}
	
		return tokens;
	
	}

	/**
	* Rendering as a text box will allow text alignment and wrap text based on width and height parameters but will not respond to all of the styling elements.
	*  This will only listen to the first styling element.
	*/
	void  TextObject::renderTextBox(std::string forcedStyle){
		TextBox textBox = TextBox();
		//Set the Text Box Size 
		textBox.setSize(Vec2i((int)mWidth, mHeight));

		//Pull out the style tags from the textstring
		std::vector<std::string> tokens = parseText();


		std::string textString("");
		StyleRef style = styleElements["DEFAULT"];

		//If a style was passed in, set the style
		if (!forcedStyle.empty()){
			style = styleElements[forcedStyle];
			if (style)setTextBoxStyle(style, textBox);
		}


		for (const auto& t : tokens) {
			if (!style &&  t.at(0) == '<' && t != "</br>"  ){//If a style is not set yet and this token starts with a tag and its not a break tag pull out  the style
					
				string tokenStyleName = t.substr(1, t.length() - 2);//Get the style name from the token by removing the '<' and  '>'
				style = styleElements[tokenStyleName]; //Try to find the style 
				if (style)setTextBoxStyle(style, textBox);		
			}

			if (t.at(0) != '<'){textBox.appendText(t);} //This token does not have and open tag so it is part of the Textstring
				
		}
		
		textBox.setLigate(true);
		textBox.setPremultiplied(true);
		mTexture = cinder::gl::Texture::create(textBox.render());

	}


	/**
	* Rendering as a text Layout will respond to different styling elements, However text rendered this way will not wrap, so </br> tags are required.
	*/
	
	void	TextObject::renderTextLayout(){
	
		TextLayout textLayout;
		
		//Pull out the style tags from the textstring
		std::vector<std::string> tokens =parseText();
		
		std::vector<StyleRef> styleStack;
		styleStack.push_back(styleElements["DEFAULT"]);

		updateTextLayoutStyle(styleStack.back(), textLayout, styleStack);
		
		TextAlignment alignment = styleStack.back()->alignment;
		std::string textLine = "";

		textLayout.clear(ColorA(0.0f, 0.0f, 0.0f, 1.0f));
		textLayout.setColor(styleStack.back()->color);

		bool hitBreakTag = false;
		bool alignmentChanged = false;
		
		for (const auto& t : tokens) {
		
			if (t.at(0) == '<'  ){//Handle Style Tag
				if (t == "</br>"){
					hitBreakTag = true;
					continue;
				}
				if (t.at(1) == '/' && styleStack.size() > 1){//THis is an end tag so Set the current style to the previous one as long as there will be at least one in the stack
				
						string tokenStyleName = t.substr(2, t.length() - 3);//Get the style name from the token	
						if (styleStack.back()->styleName == tokenStyleName){//Make sure this end tag matches the current style, if they dont match something is messed up
							styleStack.pop_back();
							updateTextLayoutStyle(styleStack.back(), textLayout, styleStack);
							
							//Set a flag if there alignment has changed
							if (styleStack.back()->alignment != alignment && styleStack.back()->alignment != AlignmentUnset){
								alignment = styleStack.back()->alignment;
								alignmentChanged = true;
							}
						}		
		
				}
				else{ // This is a new style find it and add it to the style stack
				
					string tokenStyleName = t.substr(1, t.length() - 2);//Get the style name from the token
					StyleRef nextStyle = styleElements[tokenStyleName]; //Try to find the style 
					if (nextStyle){
					
							styleStack.push_back(nextStyle);
							updateTextLayoutStyle(nextStyle, textLayout, styleStack);


							//Set a flag if there alignment has changed
							if (styleStack.back()->alignment != alignment && styleStack.back()->alignment != AlignmentUnset){
								alignment = styleStack.back()->alignment;
								alignmentChanged = true;
							}
					}
				}	
			}
			else{//Handle Text
			

				if (alignmentChanged || hitBreakTag){
					alignmentChanged = false;
					hitBreakTag = false;

					switch (alignment){
						case Left:	textLayout.addLine(t);			break;
						case Center:textLayout.addCenteredLine(t);	break;
						case Right:	textLayout.addRightLine(t);		break;
					}
				}else textLayout.append(t);
			}
		}

		//create the texture
		mTexture = cinder::gl::Texture::create(textLayout.render(true, false));
	}

	void TextObject::renderTextureFont(bool wordWrap, std::string forcedStyle ){
		
		StyleRef style;
		Font font;
		std::string textString = "";
		
		//parse the text string
		std::vector<std::string> tokens = parseText();

		//Check if we need to override the styles that are set in the text string
		if (!forcedStyle.empty()){
			style = styleElements[forcedStyle];
			if (style) font = getFont(style->fontName, style->size);
		}
		

		//Go Through 
		for (const auto& t : tokens) {

			if (t.at(0) == '<' && t != "</br>" && !style){//If this token is a style tag, and we dont already have a style 
						string tokenStyleName = t.substr(1, t.length() - 2);//Get the style name from the token
						style = styleElements[tokenStyleName]; //Try to find the style 
						if (style) font = getFont(style->fontName, style->size);
				}
			
			if (t.at(0) != '<'){ textString += t; } //This token is part of the text string
					
		}
		
        gl::TextureFont::DrawOptions drawOptions;
        drawOptions.pixelSnap(true);
		gl::TextureFontRef textureFont = gl::TextureFont::create(font);
		Rectf boundsRect(0, 0, (float)mWidth, (float)mHeight);
		

		gl::Fbo::Format format;
		format.enableMipmapping(true);
		format.setCoverageSamples(16);
		format.setSamples(16);
			
		mTextFbo = gl::Fbo(mWidth ,mHeight, format);

		mTextFbo.bindFramebuffer();
		Area viewport = gl::getViewport();

		// set viewport to the size of the FBO
		gl::setViewport(mTextFbo.getBounds());

			// store, then set the modelview and projection matrices 
			gl::pushMatrices();
				gl::setMatricesWindowPersp(mTextFbo.getSize());


				/////////////////////////////
				//Draw stufF
                gl::clear( Color( 0, 0, 0 ) ); 
				gl::enableAlphaBlending();
				if (style->colorSet)gl::color(style->color);
        if (wordWrap)textureFont->drawStringWrapped(textString, boundsRect,Vec2f::zero(),drawOptions);
				else		 textureFont->drawString(textString, boundsRect,Vec2f::zero(),drawOptions);
				/////////////////////////////

			// restore matrices
			gl::popMatrices();

		// restore viewport
		gl::setViewport(viewport);
		
		// unbind FBO
		mTextFbo.unbindFramebuffer();

		//get the texture from the fbo
		mTexture = cinder::gl::Texture::create(mTextFbo.getTexture());
		mTexture->setFlipped();
	}

	void TextObject::drawTextureFont(bool wordWrap, std::string forcedStyle){

		StyleRef style = styleElements["DEFAULT"];
		Font font = getFont(styleElements["DEFAULT"]->fontName, styleElements["DEFAULT"]->size);
		std::string textString = "";

		//parse the text string
		std::vector<std::string> tokens = parseText();

		//Check if we need to override the styles that are set in the text string
		if (!forcedStyle.empty()){
			style = styleElements[forcedStyle];
			if (style) font = getFont(style->fontName, style->size);
		}


		//Go Through 
		for (const auto& t : tokens) {

			if (t.at(0) == '<' && t != "</br>" && !style){//If this token is a style tag, and we dont already have a style 
				string tokenStyleName = t.substr(1, t.length() - 2);//Get the style name from the token
				style = styleElements[tokenStyleName]; //Try to find the style 
				if (style) font = getFont(style->fontName, style->size);
			}

			if (t.at(0) != '<'){ textString += t; } //This token is part of the text string

		}

		gl::TextureFont::DrawOptions drawOptions;
		drawOptions.pixelSnap(true);
		gl::TextureFontRef textureFont = gl::TextureFont::create(font);
		Rectf boundsRect(0, 0, (float)mWidth, (float)mHeight);



		if (style->colorSet)gl::color(style->color);
		if (wordWrap)textureFont->drawStringWrapped(textString, boundsRect, Vec2f::zero(), drawOptions);
		else		 textureFont->drawString(textString, boundsRect, Vec2f::zero(), drawOptions);
	}

	cinder::Font TextObject::getFont(std::string fontName, float fontSize){
	
		cinder::Font font;
		
		//Check the system fonts first
		if (std::find(Font::getNames().begin(), Font::getNames().end(), fontName) != Font::getNames().end()) return Font(fontName, fontSize);
		else{//Look through custom fonts in assets.

			//first check the font cache
            
			fs::path fontPath;
            #ifdef OS_WINDOWS
                fontPath= getAssetPath("Fonts\\" + fontName + ".ttf");
            #else
                fontPath= getAssetPath("Fonts/" + fontName + ".ttf");
            #endif
           

			if (FontCache[fontName])return  Font(FontCache[fontName], fontSize);
			else if (fs::exists(fontPath)){
				FontCache[fontName] = loadFile(fontPath);
				return Font(FontCache[fontName], fontSize);
			}
			else{
				
				console() << "FONT STYLE WARNING: Could Not find font: " << fontName << endl;

			}

		}

		return font;
	}
	
	void TextObject::updateTextLayoutStyle(StyleRef style, TextLayout &textLayout, const std::vector<StyleRef> &styleStack){
		
		//UPDATE THE FONT////////////////////////////////////////////////////////////////////////////
		//We are looking back through the style stack vs the just the previous settings from the textlayout for when we use this fuction after poping off a style
		if (style->fontNameSet && style->sizeSet){//Both the font and and size are set so we can just create a new font object and set the textLayout
			textLayout.setFont(getFont(style->fontName, style->size));
		}
		else if (style->fontNameSet){//only the font name changed but not the size, so we have to look back to find the last style that has the font size set
			for (auto previousStyle : boost::adaptors::reverse(styleStack)){
				if (previousStyle->sizeSet){
					textLayout.setFont(getFont(style->fontName, previousStyle->size));
					break;
				}
			}
		}
		/////////////////////////////////
		else if (style->sizeSet){//only the font size is going to change so we have to look back to find the last font that was set
			for (auto previousStyle : boost::adaptors::reverse(styleStack)){
				if (previousStyle->fontNameSet){
					textLayout.setFont(getFont(previousStyle->fontName, style->size));
					break;
				}
			}
		}
		


		if (style->leadingSet)textLayout.setLeadingOffset(style->leading);
		if (style->colorSet)textLayout.setColor(style->color);

	}

	void TextObject::setTextBoxStyle(StyleRef style, cinder::TextBox &textBox){
		
		//Set Font
		if (style->fontNameSet && style->sizeSet){//Both the font and and size are set so we can just create a new font object and set the textLayout
			textBox.setFont(getFont(style->fontName, style->size));
		}
		//Set Color
		if (style->colorSet)textBox.setColor(style->color);

		//Set alignment
		if (style->alignmentSet){
			switch (style->alignment){
				case Left:	{textBox.setAlignment(TextBox::Alignment::LEFT);	break; }
				case Center:{textBox.setAlignment(TextBox::Alignment::CENTER);	break; }
				case Right:	{textBox.setAlignment(TextBox::Alignment::RIGHT);	break; }
			}
		}
	}


	cinder::gl::TextureRef TextObject::getTexture(){

		if (mTexture) return mTexture;
		
	}



	void TextObject::destroyFbo(){
		mTextFbo.reset();
	}
	
};//End BCI Namespace