/* Copyright (c) 2010 Vladimir Chalupecky
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __MESH_H_INCLUDED__
#define __MESH_H_INCLUDED__ 

/** \file
 * \brief Routines for mesh creation and manipulation
 *
 * \author Vladimir Chalupecky
 */

#include "box2.h"
#include "mesh_types.h"

/** Allocates an empty mesh
 *
 * \return pointer to mesh data structure, NULL if allocation failed
 */
Mesh * mesh_new( void );

/** Frees a mesh from memory
 *
 * \param mesh mesh
 */
void mesh_free( Mesh *mesh );

/** Adds a node at given coordinates to the mesh.
 *
 * \param mesh mesh
 * \param x x coordinate of the node
 * \param y y coordinate of the node
 *
 * \return pointer to the newly create node, NULL if allocation failed
 */
Node * mesh_add_node( Mesh *mesh, gdouble x, gdouble y );

/** Removes a node from the mesh and frees it from memory. If the node is not
 * isolated (is connected to at least one edge), all affected edges and
 * elements are removed from the mesh as well. This can leave a hole in the
 * mesh.
 *
 * \param mesh mesh
 * \param node node to be removed from the mesh
 */
void mesh_remove_node( Mesh *mesh, Node *node );

/** Adds an edge into the mesh that connects two given nodes from the mesh. The
 * nodes have to be distinct, creation of loop edges is not allowed. There has
 * to be a free incident half-edge at each of the two nodes for the edge to be
 * added with success.
 *
 * \attention For efficiency reasons the function does not check if the added
 * edge is already present in the mesh
 *
 * \param mesh mesh
 * \param node1 first node
 * \param node2 second node
 *
 * \return pointer to the newly created edge, NULL if there was an error (no
 * memory, loop edge, no free incident half-edge)
 */
Edge * mesh_add_edge( Mesh *mesh, Node *node1, Node *node2 );

/** Removes an edge from the mesh. At the same time, it removes the two
 * triangles on each side of the edge (if they exist).
 *
 * \param mesh mesh
 * \param edge edge to be removed
 */
void mesh_remove_edge( Mesh *mesh, Edge *edge );

/** Adds an element into the mesh. The three half-edges have to be free (no
 * adjacent polygon) and have to form a chain (a target node of one half-edge is an
 * origin node of the next half-edge). The function reorders the next/previous
 * connections of the three half-edges so that they form a circular list around
 * the polygon. If such a reordering is not possible, the resulting mesh would
 * be non-manifold and the element is not added.
 *
 * \param mesh mesh
 * \param e1 first half-edge
 * \param e2 second half-edge
 * \param e3 third half-edge
 *
 * \return pointer to the newly created element, NULL if the element cannot be
 * created
 */
Element * mesh_add_element( Mesh *mesh, HalfEdge *e1, HalfEdge *e2, HalfEdge *e3 );

/** Removes an element from the mesh and frees it from memory.
 *
 * \param mesh mesh
 * \param element element to be removed
 */
void mesh_remove_element( Mesh *mesh, Element *element );

/** Swaps an edge in the mesh to the other diagonal of the quadrilateral formed
 * by the two adjacent triangles.
 *
 * \attention This function does not perform any checking. It is assumed that the
 * checks (e.g., a call to edge_is_swappable) were done by the calling function.
 * 
 * \param mesh mesh to which the edge belongs
 * \param edge edge to be swapped
 *
 * \return pointer to the swapped edge
 * 
 * \sa edge_is_swappable
 */
Edge * mesh_swap_edge( Mesh *mesh, Edge *edge );

/** Splits an element into three elements by creating three new edges between a
 * given point inside the element and element's vertices.
 *
 * \param mesh mesh containing the element
 * \param el element to split
 * \param p point in el
 *
 * \return newly created node in the element with position p
 */
Node * mesh_split_element( Mesh *mesh, Element *el, const Point2 *p );

/** Splits an edge into two subedges by creating a new node at its middle point.
 * Can handle both internal as well as boundary edges.
 *
 * \param[in] mesh mesh containing the edge
 * \param[in] edge edge to split
 * \param[out] subedge1 first subedge, can be NULL
 * \param[out] subedge2 second subedge, can be NULL
 */
void mesh_split_edge( Mesh *mesh, Edge *edge, Edge **subedge1, Edge **subedge2 );

/** Gets the axis-aligned bounding box of the mesh.
 *
 * \param[in] mesh mesh
 * \param[out] box bounding box
 */
void mesh_get_bounding_box( const Mesh *mesh, Box2 *box );

/** Finds an edge that lies at the boundary of the mesh.
 *
 * \attention The routine just iterates over the list of edges and returns the
 * first boundary edge it finds. This is not the best approach when we have to
 * call this function often. Also, if and when holes are permitted in the
 * domain, we will get access to only one boundary using this function.
 *
 * \param mesh mesh
 * 
 * \return pointer to a half-edge that lies at the "outer" side of a boundary
 * edge. In this way, we can iterate over the boundary edges using next
 * (clock-wise) and previous (counterclock-wise) pointers.
 *
 * \todo To avoid the need of iterating over all edges every time we need a
 * boundary edge, we could store a pointer to a boundary edge in the mesh
 * structrure and update it every time we modify the mesh. However, it is not
 * clear if this is advantegeous: we modify the mesh all the time but how often
 * do we need to search for a boundary edge?
 *
 * \todo If and when we allow mesh generation for domains with holes, we would
 * have to return a list of boundary half-edges
 */
HalfEdge * mesh_get_boundary_halfedge( const Mesh *mesh );

/** Locates an element in a mesh that contains a given point. This function
 * implements a simple mesh-walking algorithm. Works reliably only for convex
 * domains.
 *
 * \param[in] p point
 * \param[in] initial_element element from which the search starts
 *
 * \return element containing point p, NULL if the algorithm would walk out of
 * the mesh to reach the point (either the point lies outside the mesh or the
 * mesh domain is not convex)
 */
Element * mesh_locate_element( const Point2 *p, const Element *initial_element );

/** Removes an edge from the mesh and merges its end nodes into
 * one.
 *
 * \param mesh mesh to which the edge belongs
 * \param edge edge to be collapsed
 *
 * \return TRUE if the edge was actually collapsed, FALSE if one of the end
 * nodes lies at a boundary
 */
gboolean mesh_collapse_edge( Mesh *mesh, Edge *edge );

#endif /* __MESH_H_INCLUDED__ */

