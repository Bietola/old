#include "PictureBox.h"

///PictureBox functions
///constructors
//normal constructors
gr::PictureBox::PictureBox(const sf::FloatRect& bounds, const sf::Texture& texture, const sf::IntRect& textureRect):
    Interface(bounds),
    mTextureRect(textureRect),
    mTexture(texture)
{
    //set up vertex array
    mVertices.resize(4);
    mVertices.setPrimitiveType(sf::Quads);
}
gr::PictureBox::PictureBox(const sf::FloatRect& bounds, const sf::Texture& texture):
    PictureBox(bounds, texture, sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y))
{}
gr::PictureBox::PictureBox(const PictureBox& other):
    Interface(other),
    mTextureRect(other.mTextureRect),
    mTexture(other.mTexture),
    mVertices(other.mVertices),
    mNeedToUpdateVertices(other.mNeedToUpdateVertices)
{}

///setters
//set bounds
void gr::PictureBox::setBounds(const sf::FloatRect& bounds){
    if(getBounds() != bounds){
        //set actual bounds
        TransBox::setBounds(bounds);
        //update vertex info
        mNeedToUpdateVertices = true;
    }
}
//set texture clip rectangle
void gr::PictureBox::setTextureRect(const sf::IntRect& textureRect){
    if(getTextureRect() != textureRect){
        //set actual texture rectangle
        mTextureRect = textureRect;
        //update vertex info
        mNeedToUpdateVertices = true;
    }
}
///graphic functions
void gr::PictureBox::parentDraw(sf::RenderTarget& target, sf::RenderStates states) const {
    //update vertices if needed
    if(mNeedToUpdateVertices){
        const auto& bn = mBounds;
        const auto& tr = static_cast<sf::FloatRect>(mTextureRect);
        mVertices[0] = sf::Vertex({bn.left, bn.top},                    {tr.left, tr.top});
        mVertices[1] = sf::Vertex({bn.left+bn.width, bn.top},           {tr.left+tr.width, tr.top});
        mVertices[2] = sf::Vertex({bn.left+bn.width, bn.top+bn.height}, {tr.left+tr.width, tr.top+tr.height});
        mVertices[3] = sf::Vertex({bn.left, bn.top+bn.height},          {tr.left, tr.top+tr.height});
        mNeedToUpdateVertices = false;
    }
    //set transform
    states.transform *= getTransform();
    //set texture
    states.texture = &mTexture;
    //draw vertex array to render target
    target.draw(mVertices, states);
}
