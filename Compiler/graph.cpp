#include "link/link.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <array>
#include <cmath>
#include <iostream>
#include <array>

sf::RenderWindow window (sf::VideoMode (ScreenW , ScreenH) , "My window");

sf::Font font;
std::array<std::pair<sf::RectangleShape , sf::Text> , 5> hits;
std::array<sf::Text , 2> axis;

constexpr float OffsetW = 25;
constexpr float OffsetH = 25;

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
  Object ():
    data_ (pixels_.data ()->data ()->data_.data ())
  {
    texture_.create (SzW , SzH);
    sprite_.setTexture (texture_); // needed to draw the texture on screen
    sprite_.setPosition (OffsetW , OffsetH);
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
  if (!font.loadFromFile ("/home/mark/Programming/7/ParProga/MIPT/Compiler/FreeSans-LrmZ.ttf"))
    return 1;
  for (auto& text : axis)
  {
    text.setFont (font);
    text.setCharacterSize (15);
    text.setFillColor (sf::Color::Red);
    text.setStyle (sf::Text::Bold | sf::Text::Underlined);
  }
  axis[0].setString ("X");
  axis[0].setPosition (8 , 160);
  axis[1].setString ("Y");
  axis[1].setPosition (170 , 325);
  for (int i = 0; i < hits.size (); ++i)
  {
    hits[i].first.setPosition (340 , 10 + 50 * (i + 1));
    hits[i].first.setSize (sf::Vector2f (15 , 7));
    hits[i].second.setPosition (340 , 25 + 50 * (i + 1));
    hits[i].second.setFont (font);
    hits[i].second.setCharacterSize (15);
  }
  hits[0].first.setFillColor (sf::Color (0 , 0 , 255));
  hits[0].second.setString ("20deg");
  hits[1].first.setFillColor (sf::Color (255 , 0 , 255));
  hits[1].second.setString ("40deg");
  hits[2].first.setFillColor (sf::Color (255 , 0 , 0));
  hits[2].second.setString ("60deg");
  hits[3].first.setFillColor (sf::Color (255 , 255 , 0));
  hits[3].second.setString ("80deg");
  hits[4].first.setFillColor (sf::Color (255 , 255 , 255));
  hits[4].second.setString ("100deg");
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
  for (const auto& text : axis)
    window.draw (text);
  for (const auto& hit : hits)
  {
    window.draw (hit.first);
    window.draw (hit.second);
  }

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