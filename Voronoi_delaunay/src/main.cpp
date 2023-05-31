#include "SDL2_gfxPrimitives.h"
#include "application_ui.h"
#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <vector>
#include <random>

using namespace std;

#define EPSILON 0.0001f

struct Coords 
{
  int x, y;

  bool operator==(const Coords &other) const { return x == other.x and y == other.y; }
};

struct Segment 
{
  Coords p1, p2;
};

struct Triangle 
{
  Coords p1, p2, p3;
  bool complet = false;
};

struct GodHaveMercy
{
  Triangle t;
  Coords center;
};

struct Vertex
{
  Coords point;
  double angle;

  bool operator<(const Vertex& other) const
  {
    return angle < other.angle;
  }
};

struct Application 
{
  int width, height;
  Coords focus{100, 100};

  std::vector<Coords> points;
  std::vector<Triangle> triangles;
  std::vector<GodHaveMercy> snif;
  std::vector<std::vector<Coords>> polygons;
};

// changement => trie par x
bool compareCoords(Coords point1, Coords point2) 
{
  // Si les points x sont égaux, alors on les distingue selon leur point y
  if (point1.x == point2.x)
    return point1.y < point2.y;

  return point1.x < point2.x;
}

// changement => trie par x
bool compareCoordsReverse(Coords point1, Coords point2) 
{
  // Si les points x sont égaux, alors on les distingue selon leur point y
  if (point1.x == point2.x)
    return point1.y < point2.y;

  return point1.x > point2.x;
}

/*
   Détermine si un point se trouve dans un cercle définit par trois points
   Retourne, par les paramètres, le centre et le rayon
*/
bool CircumCircle(float pX, float pY, float x1, float y1, float x2, float y2,
                  float x3, float y3, float *xc, float *yc, float *rsqr) 
{
  float m1, m2, mx1, mx2, my1, my2;
  float dx, dy, drsqr;
  float fabsy1y2 = fabs(y1 - y2);
  float fabsy2y3 = fabs(y2 - y3);

  /* Check for coincident points */
  if (fabsy1y2 < EPSILON && fabsy2y3 < EPSILON)
    return (false);

  if (fabsy1y2 < EPSILON) {
    m2 = -(x3 - x2) / (y3 - y2);
    mx2 = (x2 + x3) / 2.0;
    my2 = (y2 + y3) / 2.0;
    *xc = (x2 + x1) / 2.0;
    *yc = m2 * (*xc - mx2) + my2;
  } else if (fabsy2y3 < EPSILON) {
    m1 = -(x2 - x1) / (y2 - y1);
    mx1 = (x1 + x2) / 2.0;
    my1 = (y1 + y2) / 2.0;
    *xc = (x3 + x2) / 2.0;
    *yc = m1 * (*xc - mx1) + my1;
  } else {
    m1 = -(x2 - x1) / (y2 - y1);
    m2 = -(x3 - x2) / (y3 - y2);
    mx1 = (x1 + x2) / 2.0;
    mx2 = (x2 + x3) / 2.0;
    my1 = (y1 + y2) / 2.0;
    my2 = (y2 + y3) / 2.0;
    *xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
    if (fabsy1y2 > fabsy2y3) {
      *yc = m1 * (*xc - mx1) + my1;
    } else {
      *yc = m2 * (*xc - mx2) + my2;
    }
  }

  dx = x2 - *xc;
  dy = y2 - *yc;
  *rsqr = dx * dx + dy * dy;

  dx = pX - *xc;
  dy = pY - *yc;
  drsqr = dx * dx + dy * dy;

  return ((drsqr - *rsqr) <= EPSILON ? true : false);
}

void drawPoints(SDL_Renderer *renderer, const std::vector<Coords> &points) 
{
  for (std::size_t i = 0; i < points.size(); i++) 
  {
    filledCircleRGBA(renderer, points[i].x, points[i].y, 3, 240, 240, 23,
                     SDL_ALPHA_OPAQUE);
  }
}

void drawSegments(SDL_Renderer *renderer,
                  const std::vector<Segment> &segments) 
{
  for (std::size_t i = 0; i < segments.size(); i++) 
  {
    lineRGBA(renderer, segments[i].p1.x, segments[i].p1.y, segments[i].p2.x,
             segments[i].p2.y, 240, 240, 20, SDL_ALPHA_OPAQUE);
  }
}

void drawTriangles(SDL_Renderer *renderer,
                   const std::vector<Triangle> &triangles) 
{
  for (std::size_t i = 0; i < triangles.size(); i++) {
    const Triangle &t = triangles[i];
    trigonRGBA(renderer, t.p1.x, t.p1.y, t.p2.x, t.p2.y, t.p3.x, t.p3.y, 0, 240,
               160, SDL_ALPHA_OPAQUE);
  }
}

void drawCircles(SDL_Renderer *renderer,
                   const std::vector<Triangle> &triangles) 
{
  float xC, yC, rC;
  for (std::size_t i = 0; i < triangles.size(); i++) 
  {
    const Triangle &t = triangles[i];
    CircumCircle(t.p1.x, t.p1.y, t.p1.x, t.p1.y, t.p2.x, t.p2.y, t.p3.x, t.p3.y,
                   &xC, &yC, &rC);
    circleRGBA(renderer, xC, yC, sqrt(rC), 30, 30, 30, SDL_ALPHA_OPAQUE);
  }
}

void drawCenterCircles(SDL_Renderer *renderer,
                   const std::vector<Triangle> &triangles) 
{
  float xC, yC, rC;
  for (std::size_t i = 0; i < triangles.size(); i++) 
  {
    const Triangle &t = triangles[i];
    CircumCircle(t.p1.x, t.p1.y, t.p1.x, t.p1.y, t.p2.x, t.p2.y, t.p3.x, t.p3.y,
                   &xC, &yC, &rC);
    filledCircleRGBA(renderer, xC, yC, 3, 240, 23, 23, SDL_ALPHA_OPAQUE);
  }
}

/*void drawPolygons(SDL_Renderer* renderer, const std::vector<std::vector<Coords>>& polygons)
{
  // Couleur des polygones
  SDL_Color color = {255, 0, 0, 255}; // Rouge

  // Dessin des polygones
  for (const std::vector<Coords>& polygon : polygons)
  {
    // Conversion des coordonnées en tableaux SDL_gfx
    std::vector<Sint16> xPoints(polygon.size());
    std::vector<Sint16> yPoints(polygon.size());

    for (std::size_t i = 0; i < polygon.size(); i++)
    {
      xPoints[i] = static_cast<Sint16>(polygon[i].x);
      yPoints[i] = static_cast<Sint16>(polygon[i].y);
    }

    // Dessin du polygone
    polygonRGBA(renderer, xPoints.data(), yPoints.data(), polygon.size(),
                color.r, color.g, color.b, color.a);
  }
}*/

void drawPolygons(SDL_Renderer* renderer, const std::vector<std::vector<Coords>>& polygons)
{
  // Générateur de nombres aléatoires pour les couleurs
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(0, 255);

  // Dessin des polygones
  for (const std::vector<Coords>& polygon : polygons)
  {
    // Génération d'une couleur aléatoire
    Uint8 r = dis(gen);
    Uint8 g = dis(gen);
    Uint8 b = dis(gen);
    Uint8 a = 255; // Opacité maximale

    // Conversion des coordonnées en tableaux SDL_gfx
    std::vector<Sint16> xPoints(polygon.size());
    std::vector<Sint16> yPoints(polygon.size());

    for (std::size_t i = 0; i < polygon.size(); i++)
    {
      xPoints[i] = static_cast<Sint16>(polygon[i].x);
      yPoints[i] = static_cast<Sint16>(polygon[i].y);
    }

    // Dessin du polygone rempli avec la couleur aléatoire
    filledPolygonRGBA(renderer, xPoints.data(), yPoints.data(), polygon.size(),
                      r, g, b, a);
  }
}

/* ********** D  R  A  W  ********** */

void draw(SDL_Renderer *renderer, const Application &app) 
{
  /* Remplissez cette fonction pour faire l'affichage du jeu */
  int width, height;
  SDL_GetRendererOutputSize(renderer, &width, &height);

  drawPoints(renderer, app.points);
  drawTriangles(renderer, app.triangles);

  // Pour dessiner le centre des cercles
  //dessiner les cercles circonscrits
  drawCircles(renderer, app.triangles);
  drawCenterCircles(renderer, app.triangles);
  // Dessinez les polygones
  drawPolygons(renderer, app.polygons);
}

/* ICI - ESPACE TRAVAIL*/

void delaunay(Application &app) 
{
  /* --- Trier les points selon x ---*/
  std::sort(app.points.begin(), app.points.end(), compareCoords);

  printf("\n\nAfter sorting vector : ");
  for (auto i = app.points.begin(); i < app.points.end(); i++) 
  {
    cout << "\n(" << i->x << ";" << i->y << ")" << endl;
  }

  /* --- Vider la liste existante de triangles ---*/
  app.triangles.clear();

  /* --- Créer un très grand triangle ---*/
  Triangle *big = new Triangle(
      {Coords{-1000, (-1000)}, Coords{500, 3000}, Coords{1500, -1000}});
  // on l'ajoute aux triangles
  app.triangles.push_back(*big);

  /* --- pour chaque point P du repère --- */
  for (auto _pt1 = app.points.begin(); _pt1 < app.points.end(); _pt1++) 
  {
    /* --- créer une liste de segments LS --- */
    vector<Segment> segments;

    /* --- chaque triangle T déjà créé --- */

    for (auto _triangle = app.triangles.begin();
         _triangle < app.triangles.end();) 
    {
      float xCircle, yCircle, rCircle;
      // si le cercle circonscrit contient le point P alors
      if(CircumCircle(_pt1->x, _pt1->y, _triangle->p1.x, _triangle->p1.y,
                   _triangle->p2.x, _triangle->p2.y, _triangle->p3.x,
                   _triangle->p3.y, &xCircle, &yCircle, &rCircle))
      {
        // Récupérer les différents segments de ce triangles dans LS
        segments.push_back({_triangle->p1, _triangle->p2});
        segments.push_back({_triangle->p2, _triangle->p3});
        segments.push_back({_triangle->p3, _triangle->p1});
        
        // Effacer le triangle de la liste
        _triangle = app.triangles.erase(_triangle);

      }
      else
      {
        ++_triangle;
      }
    }


    // pour chaque segment S de la liste LS faire
    // si un segment est le doublon d'un autre les virer
    for (auto _s1 = segments.begin(); _s1 != segments.end();)
    {
      bool erase_s1 = false;

      for (auto _s2 = segments.begin(); _s2 != segments.end();)
      {
        if (_s1 == _s2)
        {
          ++_s2;
          continue;
        }

        if (_s1->p1 == _s2->p2 && _s1->p2 == _s2->p1)
        {
          _s2 = segments.erase(_s2);
          erase_s1 = true;
        }
        else
        {
          ++_s2;
        }
      }

      if (erase_s1)
        _s1 = segments.erase(_s1);
      else
        ++_s1;
    }


    // pour chaque segment S de la liste LS faire
    for (const Segment& _seg : segments)
    {
      // créer un nouveau triangle composé du segment S et du point P
      app.triangles.push_back({_seg.p1, _seg.p2, {_pt1->x, _pt1->y}});  
    }
  }
}

void construitVoronoi(Application &app) 
{ 
  delaunay(app);

  // Vider la liste existante de snif
  app.snif.clear();

  // Vider la liste existante de polygones
  app.polygons.clear();

  // On récupère les centres des cercles de chaque triangle
  float xC, yC, rC;
  // on parcourt tous les triangles
   cout << "TAILLE TRIANGLE: " << app.triangles.size() << endl;
  for (std::size_t i = 0; i < app.triangles.size(); i++) 
  {
    const Triangle &t = app.triangles[i];
    CircumCircle(t.p1.x, t.p1.y, t.p1.x, t.p1.y, t.p2.x, t.p2.y, t.p3.x, t.p3.y,
                   &xC, &yC, &rC);
    //if((xC >= 0 && xC <=720) && (yC >= 0 && yC <= 720))
      app.snif.push_back({t, {xC, yC}});
  }

  cout << "TAILLE SNIF: " << app.snif.size() << endl;

  // On parcourt chaque point
  for (auto i = app.points.begin(); i < app.points.end(); i++) 
  {
    cout << "TAILLE POINTS: " << app.points.size() << endl;
    Coords _pt = *i;
    std::vector<Coords> polygon;
    // on parcourt chaque snif pour voir les triangles qui ont un point commun avec ce point
    for (auto j = app.snif.begin(); j < app.snif.end(); j++) 
    {
      GodHaveMercy _snif = *j;
      // Si un des points du triangle est le même que le point qu'on parcoure
      if(_pt == _snif.t.p1 || _pt == _snif.t.p2 || _pt == _snif.t.p3)
      {
        // alors le polygon est constitué de son centre de cercle
        polygon.push_back({_snif.center});
      }
    }

    // S'il y a assez de points pour faire un polygone
    if(polygon.size() >= 3)
    {
      std::vector<Coords> verifPolygon;
      std::vector<Coords> bottomPolygon;
      /* --- Trier les points DE TES MORTS DE GAUCHE A DROITE--- */
      std::sort(polygon.begin(), polygon.end(), compareCoords);

      Coords pointReference = polygon[0];
      verifPolygon.push_back(pointReference);

      int nbrPoints = 1;
      int bottomPoints = 0;

      for(size_t i = 1; i < polygon.size(); i++)
      {
        // Si le point se trouve au_dessus du point de référence
        if(polygon[i].y <= pointReference.y)
        {
          verifPolygon.push_back(polygon[i]);
          nbrPoints++;
        }
        else
        {
          bottomPolygon.push_back(polygon[i]);
          bottomPoints++;
        }
      }

      if(nbrPoints < polygon.size())
      {
        std::sort(bottomPolygon.begin(), bottomPolygon.end(), compareCoordsReverse);
        size_t i = 0;

        while(nbrPoints < polygon.size())
        {
          verifPolygon.push_back(bottomPolygon[i]);
          nbrPoints++;
          i++;
        }
      }

      app.polygons.push_back(verifPolygon);
    }
  }

  // Affichage des polygones
  for (const vector<Coords> &polygon : app.polygons) 
  {
    cout << "Polygone :" << endl;
    for (const Coords &point : polygon) 
    {
      cout << "(" << point.x << ", " << point.y << ")" << endl;
    }
    cout << endl;
  }
}

bool handleEvent(Application &app) 
{
  /* Remplissez cette fonction pour gérer les inputs utilisateurs */
  SDL_Event e;
  while (SDL_PollEvent(&e)) 
  {
    if (e.type == SDL_QUIT)
      return false;
    else if (e.type == SDL_WINDOWEVENT_RESIZED) 
    {
      app.width = e.window.data1;
      app.height = e.window.data1;
    } 
    else if (e.type == SDL_MOUSEWHEEL) 
    {
    } 
    else if (e.type == SDL_MOUSEBUTTONUP) 
    {
      if (e.button.button == SDL_BUTTON_RIGHT) 
      {
        app.focus.x = e.button.x;
        app.focus.y = e.button.y;
        app.points.clear();
      } 
      else if (e.button.button == SDL_BUTTON_LEFT) 
      {
        app.focus.y = 0;
        // Création de points
        app.points.push_back(Coords{e.button.x, e.button.y});
        construitVoronoi(app);
      }
    }
  }
  return true;
}

int main(int argc, char **argv) 
{
  SDL_Window *gWindow;
  SDL_Renderer *renderer;
  Application app{720, 720, Coords{0, 0}};
  bool is_running = true;

  // Creation de la fenetre
  gWindow = init("Awesome Voronoi", 720, 720);

  if (!gWindow) 
  {
    SDL_Log("Failed to initialize!\n");
    exit(1);
  }

  renderer = SDL_CreateRenderer(gWindow, -1, 0); // SDL_RENDERER_PRESENTVSYNC

  /* **********  G A M E   L O O P ********** */
  while (true) 
  {
    // INPUTS
    is_running = handleEvent(app);
    if (!is_running)
      break;

    // EFFACAGE FRAME
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // DESSIN
    draw(renderer, app);

    // VALIDATION FRAME
    SDL_RenderPresent(renderer);

    // PAUSE en ms
    SDL_Delay(1000 / 30);
  }

  // Free resources and close SDL
  close(gWindow, renderer);

  return 0;
}