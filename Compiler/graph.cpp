#include "link/link.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <array>
#include <cmath>
#include <iostream>

sf::RenderWindow window (sf::VideoMode (SzW , SzH) , "My window");

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
    void heat (const short heat)
    {
      if (heat < ColorPixel)
      {
        r (0);
        g (0);
        b (heat / 10);
      }
      else if (heat < ColorPixel * 2)
      {
        r ((heat - ColorPixel) / 10);
        g (0);
        b (ColorSize - 1);
      }
      else if (heat < ColorPixel * 3)
      {
        r (ColorSize - 1);
        g (0);
        b (ColorSize - 1 - (heat - ColorPixel * 2) / 10);
      }
      else if (heat < ColorPixel * 4)
      {
        r (ColorSize - 1);
        g ((heat - ColorPixel * 3) / 10);
        b (0);
      }
      else if (heat < ColorPixel * 5)
      {
        r (ColorSize - 1);
        g (ColorSize - 1);
        b ((heat - ColorPixel * 4) / 10);
      }
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
  void update (int x , int y , short cur_heat)
  {
    pixels_[x][y].heat (cur_heat);
  }
  void update (const short pixels[SzW][SzH])
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
int putPixel (int x , int y , short heat)
{
  obj.update (x , y , heat);
  return 0;
}
int flush ()
{
  if (!window.isOpen ())
    return 1;
  window.clear ();
  obj.draw ();
  sf::Event event;
  while (window.pollEvent (event))
  {
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
  }
  window.display ();
  return 0;
}