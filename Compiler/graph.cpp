#include "link/link.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <array>
#include <cmath>
#include <iostream>

sf::RenderWindow window (sf::VideoMode (SzW , SzH) , "My window");
sf::Event event;

class Object
{
  struct Pixel
  {
    std::array<sf::Uint8 , 4> data_;
    void r (int color)
    {
      data_[0] = color;
    }
    void g (int color)
    {
      data_[1] = color;
    }
    void b (int color)
    {
      data_[2] = color;
    }
    void t (int color)
    {
      data_[3] = color;
    } // transparent
    void set (int rn , int gn , int bn , int tn)
    {
      r (rn); g (gn); b (bn); t (tn);
    }
    void heat (const float heat)
    {
      r (std::round (heat * 2.55));
      g (165 - std::round (heat * 1.65));
      b (200 - std::round (heat * 2.0));
    }
  };

  std::array<std::array<Pixel , SzW> , SzH> pixels_;
  sf::Uint8* data_;
  sf::Texture texture_;
  sf::Sprite sprite_;

  public:
  Object () :
    data_ (pixels_.data ()->data ()->data_.data ())
  {
    texture_.create (SzW , SzH);
    sprite_.setTexture (texture_); // needed to draw the texture on screen
    for (auto& row : pixels_)
      for (auto& pixel : row)
        pixel.set (255 , 200 , 0 , 255);

    texture_.update (data_);
  }
  void draw ()
  {
    texture_.update (data_);
    window.draw (sprite_);
  }
  void update (int x , int y , float cur_heat)
  {
    pixels_[x][y].heat (cur_heat);
  }
  void update (const float pixels[SzW][SzH])
  {
    for (int i = 0; i < SzW; ++i)
      for (int j = 0; j < SzH; ++j)
        pixels_[i][j].heat (pixels[i][j]);
    texture_.update (data_);
  }
} obj;



int initWindow ()
{
  window.setFramerateLimit (60);
  return 0;
}
int closeWindow ()
{
  window.close ();
  return 0;
}
int putPixel (int x , int y , float heat)
{
  obj.update (x , y , heat);
  return 0;
}
int flush ()
{
  if (!window.isOpen ())
    return 1;

  while (window.pollEvent (event))
  {
    window.clear ();
    obj.draw ();
    switch (event.type)
    {
    case sf::Event::Closed:
      return 1;
    case sf::Event::KeyPressed:
      if (event.key.code == sf::Keyboard::Escape)
        return 1;
    default:
      break;
    }
    window.display ();
  }
  return 0;
}