#ifndef TEXTBOX_H_INCLUDED
#define TEXTBOX_H_INCLUDED

#include <cassert>

#include "Interface.h"

///TextBox class
class TextBox: public Interface {
    public:
        //structures
        struct TextInfo{
            std::string text;
            unsigned int charSize;
            const sf::Font* font;
        };
        //constructor
        TextBox(const sf::FloatRect& bounds);
        TextBox(const sf::FloatRect& bounds, const sf::Font* font, unsigned int charSize);
        //destructor
        ;
        //setters
        void setFont(const sf::Font* font) {mTextInfo.font = font;}
        void setCharacterSize(unsigned int size) {mTextInfo.charSize = size;}
        //text editing functions
        void setText(const std::string& text);
        void addText(const std::string& text);
        /*!WIP
        void setTextSize(unsigned int textSize);
        */
    private:
        //variables
        TextInfo mTextInfo;
        mutable sf::VertexArray mVertices;
        mutable bool mNeedToUpdateVertices;
        //functions
        void parentDraw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif // TEXTBOX_H_INCLUDED
