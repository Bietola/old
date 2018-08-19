#ifndef PICTUREBOX_H_INCLUDED
#define PICTUREBOX_H_INCLUDED

#include "Interface.h"

namespace gr {

///PictureBox class
class PictureBox: public Interface{
    public:
        ///constructors
        PictureBox(const sf::FloatRect& bounds, const sf::Texture& texture, const sf::IntRect& textureRect);
        PictureBox(const sf::FloatRect& bounds, const sf::Texture& texture);
        PictureBox(const PictureBox& other);
        ///clone
        PictureBox* clone() const override {return new PictureBox(*this);}
        ///getters
        //get texture rect
        sf::IntRect getTextureRect() const {return mTextureRect;}
        ///setters
        //set bounds
        void setBounds(const sf::FloatRect& bounds) override;
        //set texture box
        void setTextureRect(const sf::IntRect& textureRect);
    protected:
        ///variables
        //texture rect
        sf::IntRect mTextureRect;
        //texture
        const sf::Texture& mTexture;
        //vertex array
        mutable sf::VertexArray mVertices;
        //flag for updating vertex info
        mutable bool mNeedToUpdateVertices = true;
        ///interface parent functions
        //draw picture box
        void parentDraw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

}


#endif // PICTUREBOX_H_INCLUDED
