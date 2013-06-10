/*
===========================================================================

Project:   Generic Polygon Clipper

           A new algorithm for calculating the difference, intersection,
           exclusive-or or union of arbitrary polygon sets.

File:      gpc.h
Author:    Alan Murta (email: gpc@cs.man.ac.uk)
Version:   2.32
Date:      17th December 2004

Copyright: (C) Advanced Interfaces Group,
           University of Manchester.

           This software is free for non-commercial use. It may be copied,
           modified, and redistributed provided that this copyright notice
           is preserved on all copies. The intellectual property rights of
           the algorithms used reside with the University of Manchester
           Advanced Interfaces Group.

           You may not use this software, in whole or in part, in support
           of any commercial product without the express consent of the
           author.

           There is no warranty or other guarantee of fitness of this
           software for any purpose. It is provided solely "as is".

===========================================================================
*/

#ifndef __gpc_h
#define __gpc_h

#include <stdio.h>


/*
===========================================================================
                               Constants
===========================================================================
*/

/* Increase GPC_EPSILON to encourage merging of near coincident edges    */

#define GPC_EPSILON (DBL_EPSILON)

#define GPC_VERSION "2.32"


/*
===========================================================================
                           Public Data Types
===========================================================================
*/

typedef enum                        /* Set operation type                */
{
  GPC_DIFF,                         /* Difference                        */
  GPC_INT,                          /* Intersection                      */
  GPC_XOR,                          /* Exclusive or                      */
  GPC_UNION                         /* Union                             */
} gpc_op;

typedef struct                      /* Polygon vertex structure          */
{
  double              x;            /* Vertex x component                */
  double              y;            /* vertex y component                */
} gpc_vertex;

typedef struct                      /* Vertex list structure             */
{
  int                 num_vertices; /* Number of vertices in list        */
  gpc_vertex         *vertex;       /* Vertex array pointer              */
} gpc_vertex_list;

typedef struct                      /* Polygon set structure             */
{
  int                 num_contours; /* Number of contours in polygon     */
  int                *hole;         /* Hole / external contour flags     */
  gpc_vertex_list    *contour;      /* Contour array pointer             */
} gpc_polygon;

typedef struct                      /* Tristrip set structure            */
{
  int                 num_strips;   /* Number of tristrips               */
  gpc_vertex_list    *strip;        /* Tristrip array pointer            */
} gpc_tristrip;


/*
===========================================================================
                       Public Function Prototypes
===========================================================================
*/

void gpc_read_polygon        (FILE            *infile_ptr,
                              int              read_hole_flags,
                              gpc_polygon     *polygon);

void gpc_write_polygon       (FILE            *outfile_ptr,
                              int              write_hole_flags,
                              gpc_polygon     *polygon);

void gpc_add_contour         (gpc_polygon     *polygon,
                              gpc_vertex_list *contour,
                              int              hole);

void gpc_polygon_clip        (gpc_op           set_operation,
                              gpc_polygon     *subject_polygon,
                              gpc_polygon     *clip_polygon,
                              gpc_polygon     *result_polygon);

void gpc_tristrip_clip       (gpc_op           set_operation,
                              gpc_polygon     *subject_polygon,
                              gpc_polygon     *clip_polygon,
                              gpc_tristrip    *result_tristrip);

void gpc_polygon_to_tristrip (gpc_polygon     *polygon,
                              gpc_tristrip    *tristrip);

void gpc_free_polygon        (gpc_polygon     *polygon);

void gpc_free_tristrip       (gpc_tristrip    *tristrip);

/*
===========================================================================
                       CGenPolygon Class
===========================================================================
*/

class CGenPolygon;

class CGenPolygonVertexRef {
 public:
  CGenPolygonVertexRef(const CGenPolygon *polygon, uint ind1, uint ind2) :
   polygon_(polygon), ind1_(ind1), ind2_(ind2) {
  }

  double getX() const;
  double getY() const;

 private:
  const CGenPolygon *polygon_;
  uint               ind1_;
  uint               ind2_;
};

class CGenPolygonContourRef {
 public:
  CGenPolygonContourRef(const CGenPolygon *polygon, uint ind) :
   polygon_(polygon), ind_(ind) {
  }

  uint getNumVertices() const;

  CGenPolygonVertexRef getVertex(uint i) const {
    return CGenPolygonVertexRef(polygon_, ind_, i);
  }

 private:
  const CGenPolygon *polygon_;
  uint               ind_;
};

class CGenPolygon {
 public:
  CGenPolygon() {
    polygon_ = (gpc_polygon *) calloc(1, sizeof(gpc_polygon));
  }

 ~CGenPolygon() {
    gpc_free_polygon(polygon_);
  }

  void read(FILE *fp) {
    gpc_read_polygon(fp, 0, polygon_);
  }

  uint getNumContours() const {
    return polygon_->num_contours;
  }

  CGenPolygonContourRef getContour(uint i) const {
    return CGenPolygonContourRef(this, i);
  }

  CGenPolygonVertexRef getVertex(uint i, uint j) const {
    return CGenPolygonVertexRef(this, i, j);
  }

  void addPolygon(const double *x, const double *y, uint num_xy) {
    gpc_vertex_list *vertices = (gpc_vertex_list *) calloc(1, sizeof(gpc_vertex_list));;

    vertices->num_vertices = num_xy;
    vertices->vertex       = (gpc_vertex *) calloc(num_xy, sizeof(gpc_vertex));

    for (uint i = 0; i < num_xy; ++i) {
      vertices->vertex[i].x = x[i];
      vertices->vertex[i].y = y[i];
    }

    gpc_add_contour(polygon_, vertices, 0);
  }

  bool orOp(const CGenPolygon &clip, CGenPolygon &result) const {
    // union
    gpc_polygon_clip(GPC_UNION, this->polygon_, clip.polygon_, result.polygon_);

    return (result.polygon_->num_contours > 0);
  }

  bool andOp(const CGenPolygon &clip, CGenPolygon &result) const {
    // intersection
    gpc_polygon_clip(GPC_INT, this->polygon_, clip.polygon_, result.polygon_);

    return (result.polygon_->num_contours > 0);
  }

  bool xorOp(const CGenPolygon &clip, CGenPolygon &result) const {
    // xor
    gpc_polygon_clip(GPC_XOR, this->polygon_, clip.polygon_, result.polygon_);

    return (result.polygon_->num_contours > 0);
  }

  bool diffOp(const CGenPolygon &clip, CGenPolygon &result) const {
    // difference
    gpc_polygon_clip(GPC_DIFF, this->polygon_, clip.polygon_, result.polygon_);

    return (result.polygon_->num_contours > 0);
  }

 protected:
  friend class CGenPolygonContourRef;
  friend class CGenPolygonVertexRef;

  uint getNumVertices(uint i) const {
    return polygon_->contour[i].num_vertices;
  }

  double getX(uint i, uint j) const {
    return polygon_->contour[i].vertex[j].x;
  }

  double getY(uint i, uint j) const {
    return polygon_->contour[i].vertex[j].y;
  }

 private:
  gpc_polygon *polygon_;
};

#endif

/*
===========================================================================
                           End of file: gpc.h
===========================================================================
*/
