#include "TextBox.h"

///TextBox functions
//constructors
gr::TextBox::TextBox(const sf::FloatRect& bounds, const sf::Font* font, unsigned int charSize):
    Interface(bounds),
    mTextInfo{"", charSize, font}
{
    mVertices.setPrimitiveType(sf::Quads);
}
gr::TextBox::TextBox(const sf::FloatRect& bounds):
    TextBox(bounds, nullptr, 10)
{}
gr::TextBox::TextBox(const gr::TextBox& other):
    Interface(other),
    mTextInfo(other.mTextInfo),
    mVertices(other.mVertices),
    mNeedToUpdateVertices(other.mNeedToUpdateVertices)
{}

//set text
void gr::TextBox::setText(const std::string& text) {
    mTextInfo.text = text;
    mNeedToUpdateVertices = true;
}
//add text
void gr::TextBox::addText(const std::string& text) {
    mTextInfo.text += text;
    mNeedToUpdateVertices = true;
}
//draw
void gr::TextBox::parentDraw(sf::RenderTarget& target, sf::RenderStates states) const {
    //if no font is set, abort
    assert(mTextInfo.font);
    //update vertices if necessary
    if(mNeedToUpdateVertices){
        std::cout << "text: " << mTextInfo.text << "\n\n";
        mNeedToUpdateVertices = false;
        //calculate new vertex array size needed
        //(updated constantly when special (not printable) characters
        // are handled and used at the end to resize the vertex array)
        size_t verticesNum = 0;
        for(const auto& ele : mTextInfo.text) verticesNum++;
        mVertices.resize(verticesNum * 4);
        //adjust vertex array
        const auto& newText = mTextInfo.text;
        unsigned int lineOffset = getBounds().left;
        unsigned int verticalOffset = getBounds().top + mTextInfo.font->getLineSpacing(mTextInfo.charSize);
        for(size_t j = 0, vj = 0; j < newText.size(); j++, vj++){
            //handle special characters
            if(newText[j] == '\n'){
                verticesNum--;
                vj--;
                lineOffset = getBounds().left;
                verticalOffset += mTextInfo.font->getLineSpacing(mTextInfo.charSize);
            }
            //handle letters (non-handled characters are rendered as letters)
            else {
                //handle space
                if(newText[j] == ' '){
                    //generate a new line
                    auto futLineOffset = lineOffset;
                    auto k = j + 1;
                    for(; newText[k] != ' ' && newText[k] != '\n' && k < newText.size(); k++){
                        futLineOffset += mTextInfo.font->getGlyph(newText[k], mTextInfo.charSize, false).advance;
                    }
                    if(futLineOffset > getBounds().width){
                        verticesNum--;
                        j++;
                        lineOffset = getBounds().left;
                        verticalOffset += mTextInfo.font->getLineSpacing(mTextInfo.charSize);
                    }
                }
                //change vertex quad associated with letter
                auto glyph = mTextInfo.font->getGlyph(newText[j], mTextInfo.charSize, false);
                auto bRect = glyph.bounds;
                float kerning = 0;
                //handle kerning
                if(j < newText.size() - 1)
                    kerning = mTextInfo.font->getKerning(newText[j], newText[j+1], mTextInfo.charSize);
                //set glyph bounds within text box
                bRect.left = lineOffset + kerning;
                lineOffset += glyph.advance + kerning;
                bRect.top += verticalOffset;
                //get texture rect
                const auto& tRect = glyph.textureRect;
                //convert glyph into vertex quad
                mVertices[vj*4]      = sf::Vertex({bRect.left, bRect.top},                               {tRect.left, tRect.top});
                mVertices[vj*4 + 1]  = sf::Vertex({bRect.left + bRect.width, bRect.top},                 {tRect.left + tRect.width, tRect.top});
                mVertices[vj*4 + 2]  = sf::Vertex({bRect.left + bRect.width, bRect.top + bRect.height},  {tRect.left + tRect.width, tRect.top + tRect.height});
                mVertices[vj*4 + 3]  = sf::Vertex({bRect.left, bRect.top + bRect.height},                {tRect.left, tRect.top + tRect.height});
            }
            //handle horizontal overflow
            if(lineOffset >= getBounds().width){
                lineOffset = getBounds().left;
                verticalOffset += mTextInfo.font->getLineSpacing(mTextInfo.charSize);
            }
        }
        //resize vertex array if necessary (special characters were encountered)
        //if(mVertices.getVertexCount() != verticesNum * 4);
        //    mVertices.resize(verticesNum * 4);
    }
    //set transform
    states.transform *= getTransform();
    //set texture
    states.texture = &(mTextInfo.font->getTexture(mTextInfo.charSize));
    //draw vertex array to render target
    target.draw(mVertices, states);
}
