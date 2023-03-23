#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <map>

class HGraph
{

  using Connections = std::vector<std::vector<unsigned>>;

  Connections Vertices;
  Connections Edges;

  public:
  HGraph (std::ifstream& FIn);

  void dump (std::ostream& Out = std::cout) const;

  Connections const& getVertices () const { return Vertices; }
  Connections const& getEdges () const { return Edges; }
};

class Partitions
{
  std::vector<bool> VertPartitions;
  HGraph const& Graph;

  int Cost { 0 };
  int Side { 0 };

  void calculateCost ();

  public:
  void setCost (int CostIn) { Cost = CostIn; }
  int getCost () const { return Cost; }
  int getSide () const { return Side; }

  Partitions (HGraph const GraphIn);
  void dump (std::ostream& Out = std::cout) const;
  void out (std::ostream& Out = std::cout) const;
  void update (unsigned Vertex);

  std::vector<bool> const& getPart () const { return VertPartitions; }
};

class GainContainer
{
  using RLSide = std::map<int , std::set<int>>;
  using Move = std::pair<unsigned , int>;

  RLSide Left;
  RLSide Right;

  std::vector<int> VertGain;
  std::set<unsigned> IsDeleted;

  RLSide& getNeededSide (bool Side);
  RLSide const& getNeededSide (bool Side) const;

  public:
  GainContainer (HGraph const& HG , Partitions const& Prt);
  bool isEmpty (bool Check) const;
  Move bestFeasibleMove (bool Side);
  void update (unsigned Vertex , bool Side , int Value);
  void erase (unsigned Vertex , bool Side);
  void updateDeleted (unsigned Vertex);

  void dump (std::ostream& Out = std::cout) const;
};

// MAIN ALGO
unsigned FM (HGraph const& , Partitions&);