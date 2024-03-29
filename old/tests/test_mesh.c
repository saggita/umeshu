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

#include <edge.h>
#include <mesh.h>
#include <mesh_delaunay.h>
#include <mesh_output.h>
#include <point2.h>
#include <polygon_triangulate.h>

int test_mesh( int argc, char *argv[] )
{
    Mesh *mesh;
    Node *n1, *n2, *n3, *n4;
    Edge *e1, *e2, *e3, *e4, *e5;
    Element *el1, *el2;
    
    mesh = mesh_new();
    g_return_val_if_fail( mesh->Np == 0, 1 );
    g_return_val_if_fail( mesh->Ne == 0, 1 );
    g_return_val_if_fail( mesh->Nt == 0, 1 );
    g_return_val_if_fail( mesh->nodes == NULL, 1 );
    g_return_val_if_fail( mesh->edges == NULL, 1 );
    g_return_val_if_fail( mesh->elements == NULL, 1 );

    n1 = mesh_add_node( mesh, 1.0, 2.0 );
    g_return_val_if_fail( mesh->Np == 1, 1 );
    g_return_val_if_fail( mesh->Ne == 0, 1 );
    g_return_val_if_fail( mesh->Nt == 0, 1 );
    g_return_val_if_fail( mesh->nodes != NULL, 1 );
    g_return_val_if_fail( mesh->edges == NULL, 1 );
    g_return_val_if_fail( mesh->elements == NULL, 1 );

    n2 = mesh_add_node( mesh, 0.0, 1.0 );
    n3 = mesh_add_node( mesh, 1.0, 0.0 );
    n4 = mesh_add_node( mesh, 2.0, 1.0 );

    e1 = mesh_add_edge( mesh, n1, n2 );
    g_return_val_if_fail( mesh->Np == 4, 1 );
    g_return_val_if_fail( mesh->Ne == 1, 1 );
    g_return_val_if_fail( mesh->Nt == 0, 1 );
    g_return_val_if_fail( mesh->nodes != NULL, 1 );
    g_return_val_if_fail( mesh->edges != NULL, 1 );
    g_return_val_if_fail( mesh->elements == NULL, 1 );

    e2 = mesh_add_edge( mesh, n2, n3 );
    e3 = mesh_add_edge( mesh, n3, n4 );
    e4 = mesh_add_edge( mesh, n4, n1 );
    e5 = mesh_add_edge( mesh, n1, n3 );

    el1 = mesh_add_element( mesh, &e1->he[0], &e2->he[0], &e5->he[1] );
    el2 = mesh_add_element( mesh, &e5->he[0], &e3->he[0], &e4->he[0] );
    g_return_val_if_fail( mesh->Np == 4, 1 );
    g_return_val_if_fail( mesh->Ne == 5, 1 );
    g_return_val_if_fail( mesh->Nt == 2, 1 );
    g_return_val_if_fail( mesh->nodes != NULL, 1 );
    g_return_val_if_fail( mesh->edges != NULL, 1 );
    g_return_val_if_fail( mesh->elements != NULL, 1 );

    mesh_remove_node( mesh, n1 );
    g_return_val_if_fail( mesh->Np == 3, 1 );
    g_return_val_if_fail( mesh->Ne == 2, 1 );
    g_return_val_if_fail( mesh->Nt == 0, 1 );
    g_return_val_if_fail( mesh->nodes != NULL, 1 );
    g_return_val_if_fail( mesh->edges != NULL, 1 );
    g_return_val_if_fail( mesh->elements == NULL, 1 );
    mesh_free( mesh );

    mesh = mesh_new();
    n1 = mesh_add_node( mesh, 1.0, 2.0 );
    n2 = mesh_add_node( mesh, 0.0, 1.0 );
    n3 = mesh_add_node( mesh, 1.0, 0.0 );
    n4 = mesh_add_node( mesh, 2.0, 1.0 );
    e1 = mesh_add_edge( mesh, n1, n2 );
    e2 = mesh_add_edge( mesh, n2, n3 );
    e3 = mesh_add_edge( mesh, n3, n4 );
    e4 = mesh_add_edge( mesh, n4, n1 );
    e5 = mesh_add_edge( mesh, n1, n3 );
    el1 = mesh_add_element( mesh, &e1->he[0], &e2->he[0], &e5->he[1] );
    el2 = mesh_add_element( mesh, &e5->he[0], &e3->he[0], &e4->he[0] );
    mesh_remove_edge( mesh, e5 );
    g_return_val_if_fail( mesh->Np == 4, 1 );
    g_return_val_if_fail( mesh->Ne == 4, 1 );
    g_return_val_if_fail( mesh->Nt == 0, 1 );
    g_return_val_if_fail( mesh->nodes != NULL, 1 );
    g_return_val_if_fail( mesh->edges != NULL, 1 );
    g_return_val_if_fail( mesh->elements == NULL, 1 );
    mesh_free( mesh );

    mesh = mesh_new();
    n1 = mesh_add_node( mesh, 1.0, 2.0 );
    n2 = mesh_add_node( mesh, 0.0, 1.0 );
    n3 = mesh_add_node( mesh, 1.0, 0.0 );
    n4 = mesh_add_node( mesh, 2.0, 1.0 );
    e1 = mesh_add_edge( mesh, n1, n2 );
    e2 = mesh_add_edge( mesh, n2, n3 );
    e3 = mesh_add_edge( mesh, n3, n4 );
    e4 = mesh_add_edge( mesh, n4, n1 );
    e5 = mesh_add_edge( mesh, n1, n3 );
    el1 = mesh_add_element( mesh, &e1->he[0], &e2->he[0], &e5->he[1] );
    el2 = mesh_add_element( mesh, &e5->he[0], &e3->he[0], &e4->he[0] );
    mesh_remove_element( mesh, el1 );
    mesh_remove_element( mesh, el2 );
    g_return_val_if_fail( mesh->Np == 4, 1 );
    g_return_val_if_fail( mesh->Ne == 5, 1 );
    g_return_val_if_fail( mesh->Nt == 0, 1 );
    g_return_val_if_fail( mesh->nodes != NULL, 1 );
    g_return_val_if_fail( mesh->edges != NULL, 1 );
    g_return_val_if_fail( mesh->elements == NULL, 1 );
    mesh_free( mesh );

    mesh = mesh_new();
    n1 = mesh_add_node( mesh, 0.0, 0.0 );
    n2 = mesh_add_node( mesh, 1.0, 0.0 );
    n3 = mesh_add_node( mesh, 1.0, 1.0 );
    n4 = mesh_add_node( mesh, 0.0, 1.0 );
    e1 = mesh_add_edge( mesh, n1, n2 );
    e2 = mesh_add_edge( mesh, n2, n3 );
    e3 = mesh_add_edge( mesh, n3, n4 );
    e4 = mesh_add_edge( mesh, n4, n1 );
    e5 = mesh_add_edge( mesh, n1, n3 );
    mesh_add_element( mesh, &e1->he[0], &e2->he[0], &e5->he[1] );
    mesh_add_element( mesh, &e5->he[0], &e3->he[0], &e4->he[0] );
    g_return_val_if_fail( edge_is_swappable( e5 ), 1 );
    e5 = mesh_swap_edge( mesh, e5 );
    g_return_val_if_fail( mesh->Np == 4, 1 );
    g_return_val_if_fail( mesh->Ne == 5, 1 );
    g_return_val_if_fail( mesh->Nt == 2, 1 );
    g_return_val_if_fail( mesh->nodes != NULL, 1 );
    g_return_val_if_fail( mesh->edges != NULL, 1 );
    g_return_val_if_fail( mesh->elements != NULL, 1 );

    Point2 p;
    point2_set( &p, 0.25, 0.25 );
    Node * n5 = mesh_split_element( mesh, e1->he[0].element, &p );
    g_return_val_if_fail( mesh->Np == 5, 1 );
    g_return_val_if_fail( mesh->Ne == 8, 1 );
    g_return_val_if_fail( mesh->Nt == 4, 1 );
    g_return_val_if_fail( mesh->nodes != NULL, 1 );
    g_return_val_if_fail( mesh->edges != NULL, 1 );
    g_return_val_if_fail( mesh->elements != NULL, 1 );
    g_return_val_if_fail( NODE_POSITION(n5)->x == p.x, 1 );
    g_return_val_if_fail( NODE_POSITION(n5)->y == p.y, 1 );

    mesh_save_to_eps( "test_mesh_2_before_edge_split.eps", mesh );
    mesh_split_edge( mesh, e5, NULL, NULL );
    g_return_val_if_fail( mesh->Np == 6, 1 );
    g_return_val_if_fail( mesh->Ne == 11, 1 );
    g_return_val_if_fail( mesh->Nt == 6, 1 );
    g_return_val_if_fail( mesh->nodes != NULL, 1 );
    g_return_val_if_fail( mesh->edges != NULL, 1 );
    g_return_val_if_fail( mesh->elements != NULL, 1 );
    mesh_save_to_eps( "test_mesh_2_after_edge_split.eps", mesh );
    mesh_split_edge( mesh, e2, NULL, NULL );
    g_return_val_if_fail( mesh->Np == 7, 1 );
    g_return_val_if_fail( mesh->Ne == 13, 1 );
    g_return_val_if_fail( mesh->Nt == 7, 1 );
    g_return_val_if_fail( mesh->nodes != NULL, 1 );
    g_return_val_if_fail( mesh->edges != NULL, 1 );
    g_return_val_if_fail( mesh->elements != NULL, 1 );
    mesh_save_to_eps( "test_mesh_2_after_bedge_split.eps", mesh );

    Box2 box;
    mesh_get_bounding_box( mesh, &box );
    g_return_val_if_fail( box.min[0] == 0.0, 1 );
    g_return_val_if_fail( box.min[1] == 0.0, 1 );
    g_return_val_if_fail( box.max[0] == 1.0, 1 );
    g_return_val_if_fail( box.max[1] == 1.0, 1 );
    mesh_add_node( mesh, 2.0, 3.0 );
    mesh_get_bounding_box( mesh, &box );
    g_return_val_if_fail( box.min[0] == 0.0, 1 );
    g_return_val_if_fail( box.min[1] == 0.0, 1 );
    box2_print( &box );
    g_return_val_if_fail( box.max[0] == 2.0, 1 );
    g_return_val_if_fail( box.max[1] == 3.0, 1 );

    mesh_free( mesh );

    return 0;
}


