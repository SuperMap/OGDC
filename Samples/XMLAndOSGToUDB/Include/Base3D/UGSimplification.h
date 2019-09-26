//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file headerfile.h
//!  \brief
//!  \details
//!  \author 。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGSIMPLIFICATIONVERTEX_H__INCLUDED_)
#define		 AFX_UGSIMPLIFICATIONVERTEX_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#pragma warning(disable:4710) // function not inlined
#pragma warning(disable:4702) // unreachable code
#pragma warning(disable:4514) // unreferenced inline function has been removed
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif // _MSC_VER > 1000

#include "Base3D/UGPrerequisites3D.h"
#include "Base3D/UGVector4d.h"
#include <vector>
#include <set>
#include <assert.h>

using namespace std;



namespace UGC 
{
	class UGSimplificationVertex;
	class UGSimplificationTriangle;
	class UGSimplificationMesh;
	
	
	struct border
	{
		UGint vert1;
		UGint vert2;
		UGint triIndex;

		UGbool operator<(const border& b) const 
		{
			UGint v1, v2, b1, b2;
			if (vert1 < vert2) 
			{
				v1 = vert1; 
				v2 = vert2;
			}
			else
			{
				v1 = vert2; 
				v2 = vert1;
			}
			if (b.vert1 < b.vert2) 
			{
				b1 = b.vert1; 
				b2 = b.vert2;
			} 
			else 
			{
				b1 = b.vert2; 
				b2 = b.vert1;
			}
			if (v1 < b1) 
			{
				return TRUE;
			}
			if (v1 > b1) 
			{
				return FALSE;
			}
			return (v2 < b2); // v1 == b1
		}
	};
	//vertex:
	class BASE3D_API UGSimplificationVertex  
	{
	public:
		
		UGSimplificationVertex() : _myVertex( 0.0, 0.0, 0.0 ), //_vertexNormal( 0.0, 0.0, 0.0 ), 
								   _bActive( FALSE ), _cost( 0 ), _minCostNeighbor( -1 ),
								   _index( -1 )//, _QTriArea( 0 )
		  {
//			  initQuadric();
		  };

		  UGSimplificationVertex( UGdouble x1, UGdouble y1, UGdouble z1) : _myVertex( x1, y1, z1 ), 
															//			   _vertexNormal( 0.0, 0.0, 0.0 ), 
																		   _bActive( TRUE ), _cost( 0 ), _minCostNeighbor( -1 ),
																		   _index( -1 )//, _QTriArea( 0 )
		  {
//			  initQuadric();
		  };

		  UGSimplificationVertex( UGdouble av[3] ): _myVertex( av[0], av[1], av[2] ), //_vertexNormal( 0.0, 0.0, 0.0 ), 
													_bActive( TRUE ), _cost( 0 ), _minCostNeighbor( -1 ),
													_index( -1 )//, _QTriArea( 0 )
		  {
//			  initQuadric();
		  };

		  UGSimplificationVertex(UGdouble av[3], UGdouble vn[3]): _myVertex( av[0], av[1], av[2] ), //_vertexNormal( vn[0], vn[1], vn[2] ), 
																  _bActive( TRUE ), _cost( 0 ), _minCostNeighbor( -1 ),
																  _index( -1 )//, _QTriArea( 0 )
		  {
//			  initQuadric();
		  };

		  // copy ctor
		  UGSimplificationVertex(const UGSimplificationVertex& v) : _myVertex( v._myVertex ), //_vertexNormal( v._vertexNormal ),
																	_vertNeighbors( v._vertNeighbors ), _triNeighbors( v._triNeighbors ),
																	_bActive( v._bActive ), _cost( v._cost ), 
																	_minCostNeighbor( v._minCostNeighbor ),
																	_index( v._index )//, _QTriArea( v._QTriArea )
		  {
			  // copy quadric
// 			  for (UGint i = 0; i < 4; ++i) 
// 			  {
// 				  for (UGint j = 0; j < 4; ++j) 
// 				  {
// 					  _Q[i][j] = v._Q[i][j];
// 				  }
// 			  }		
		  };

		  // destructor
		  ~UGSimplificationVertex() 
		  {
			  _vertNeighbors.erase(_vertNeighbors.begin(), _vertNeighbors.end());
			  _triNeighbors.erase(_triNeighbors.begin(), _triNeighbors.end());
		  };

		  // Assignment operator
		  UGSimplificationVertex& operator=(const UGSimplificationVertex& v) 
		  {
			  if (this == &v) return *this; // check for assignment to self
			  _myVertex =v._myVertex;
// 			  _vertexNormal = v._vertexNormal; 
			  _vertNeighbors = v._vertNeighbors;
			  _triNeighbors = v._triNeighbors;
			  _bActive = v._bActive;
			  _cost = v._cost;
			  _minCostNeighbor = v._minCostNeighbor;
			  _index = v._index;
// 			  _QTriArea = v._QTriArea;
			  // copy quadric
// 			  for (UGint i = 0; i < 4; ++i) 
// 			  {
// 				  for (UGint j = 0; j < 4; ++j) 
// 				  {
// 					  _Q[i][j] = v._Q[i][j];
// 				  }
// 			  }
			  return *this;
		  };
		  // Assignment operator
		  UGSimplificationVertex& operator=(const UGdouble av[3])
		  {
			  _myVertex.x=av[0];_myVertex.y=av[1];_myVertex.z=av[2];
			  // erase the list of neighboring vertices, faces
			  // since we're copying from an array of floats
			  _vertNeighbors.erase(_vertNeighbors.begin(), _vertNeighbors.end());
			  _triNeighbors.erase(_triNeighbors.begin(), _triNeighbors.end());
			  _cost = 0;
			  _minCostNeighbor = -1;
			  _index = -1;
// 			  _QTriArea = 0;
// 			  initQuadric();
			  return *this;
		  };

		  // Comparision operators
		  bool operator==(const UGSimplificationVertex& v) 
		  {
			  return (_myVertex == v._myVertex /*&& _vertexNormal == v._vertexNormal*/);
		  }
		  bool operator!=(const UGSimplificationVertex& v) 
		  {
			  return (_myVertex != v._myVertex /*|| _vertexNormal != v._vertexNormal*/);
		  }

		  const UGdouble* getArrayVerts() const 
		  {
			  _v[0]=_myVertex.x;
			  _v[1]=_myVertex.y;
			  _v[2]=_myVertex.z;
			  return _v;
		  }

// 		  const UGdouble* getArrayVertNorms() const 
// 		  {
// 			  _vn[0]=_vertexNormal.x;
// 			  _vn[1]=_vertexNormal.y;
// 			  _vn[2]=_vertexNormal.z;
// 			  return _vn;
// 		  }

		  // a vertex neighbor is connected by an edge
		  void addVertNeighbor(UGint v)
		  {
			  _vertNeighbors.insert(v);
		  }

		  // remove a vertex which is no longer connected by an edge
		  UGuint removeVertNeighbor(UGint v) 
		  {
			  return _vertNeighbors.erase(v);
		  }

		  // a triangle neighbor is a triangle which uses this vertex
		  void addTriNeighbor(UGint t)
		  {
			  _triNeighbors.insert(t);
		  }

		  // remove triangle if it no longer uses this vertex
		  UGuint  removeTriNeighbor(UGint t) 
		  {
			  return _triNeighbors.erase(t);
		  }

		  UGVector3d& getXYZ() 
		  {
			  return _myVertex;
		  }
		  const UGVector3d& getXYZ() const 
		  {
			  return _myVertex;
		  }

		  // if a vertex is removed, we set a flag
		  UGbool isActive() const 
		  {
			  return _bActive;
		  }
		  void setActive(UGbool b) 
		  {
			  _bActive = b;
		  }

		  std::set<UGint>& getVertNeighbors()
		  {
			  return _vertNeighbors;
		  }
		  std::set<UGint>& getTriNeighbors() 
		  {
			  return _triNeighbors;
		  }

		  UGbool hasVertNeighbor(UGint v) 
		  {
			  return (_vertNeighbors.find(v) != _vertNeighbors.end());
		  }
		  UGbool hasTriNeighbor(UGint t)  
		  {
			  return (_triNeighbors.find(t) != _triNeighbors.end());
		  }

		  // edge remove costs are used in mesh simplification
		  UGdouble edgeRemoveCost() 
		  {
			  return _cost;
		  }
		  void setEdgeRemoveCost(UGdouble f) 
		  {
			  _cost = f;
		  }

		  UGint minCostEdgeVert() const 
		  {
			  return _minCostNeighbor;
		  }
		  void setMinCostEdgeVert(UGint i) 
		  {
			  _minCostNeighbor = i;
		  }

		  UGdouble getCost() const 
		  {
			  return _cost;
		  }

		  // operator< & operator> are used to order vertices by edge removal costs
		  UGbool operator<(const UGSimplificationVertex& v) const 
		  {
			  return (_cost < v._cost);
		  }
		  UGbool operator>(const UGSimplificationVertex& v) const 
		  {
			  return (_cost > v._cost);
		  }

		  UGint getIndex() const 
		  {
			  return _index;
		  }
		  void setIndex(UGint i) 
		  {
			  _index = i;
		  }

		  // Used for Garland & Heckbert's quadric edge collapse cost (used for mesh simplifications/progressive meshes)
//		  void calcQuadric(UGSimplificationMesh& m, UGbool bUseTriArea); // calculate the 4x4 Quadric matrix

// 		  void getQuadric(UGdouble Qret[4][4]) 
// 		  {
// 			  for (UGint i = 0; i < 4; ++i) 
// 			  {
// 				  for (UGint j = 0; j < 4; ++j) 
// 				  {
// 					  Qret[i][j] = _Q[i][j];
// 				  }
// 			  }
// 		  }

// 		  void setQuadric(UGdouble Qnew[4][4]) 
// 		  {
// 			  for (UGint i = 0; i < 4; ++i) 
// 			  {
// 				  for (UGint j = 0; j < 4; ++j) 
// 				  {
// 					  _Q[i][j] = Qnew[i][j];
// 				  }
// 			  }
// 		  }


		  UGbool isBorder(UGSimplificationMesh& m); // is this vertex on the border 
		  // (i.e. is there an edge which uses this vertex which 
		  // is only used for one triangle?)

		  // Used for Gouraud shading
// 		  void setVertNomal(const UGVector3d& vn) 
// 		  {
// 			  _vertexNormal = vn;
// 		  };

// 		  UGdouble getQuadricSummedTriArea() 
// 		  {
// 			  return _QTriArea;
// 		  }
// 		  void setQuadricSummedTriArea(UGdouble newArea) 
// 		  {
// 			  _QTriArea = newArea;
// 		  }

		  // Is the current vertex on an edge?  If so, get edge information.
		  // This is used to put constraints on the border so that the mesh
		  // edges aren't "eaten away" by the mesh simplification.
		  void getAllBorderEdges(std::set<border> &borderSet, UGSimplificationMesh& m);

		  UGVector3d _myVertex;     // X, Y, Z position of this vertex
// 		  UGVector3d _vertexNormal; // vertex normal, used for Gouraud shading

		  std::set<UGint> _vertNeighbors; // connected to this vertex via an edge
		  std::set<UGint> _triNeighbors;  // triangles of which this vertex is a part

		  UGbool     _bActive;          // false if vertex has been removed

		  UGdouble _cost;            // cost of removing this vertex from Progressive Mesh
		  UGint      _minCostNeighbor; // index of vertex at other end of the min. cost edge

		  UGint		 _index;

		  mutable UGdouble _v[3];
// 		  mutable UGdouble _vn[3];

// 		  UGdouble		 _Q[4][4];  // Used for Quadric error cost.

// 		  UGdouble		 _QTriArea; // summed area of triangles used to computer quadrics

// 		  void initQuadric()
// 		 {
// 		 	for (UGint i = 0; i < 4; ++i)
// 		 	{
// 				for (UGint j = 0; j < 4; ++j) 
// 				{
// 					_Q[i][j] = -1;
// 				}
// 			}
// 		 }
	 };

	//triangle:
	class BASE3D_API UGSimplificationTriangle
	{
	public:
		// Constructors
		UGSimplificationTriangle() : 
			  _vert1(-1), _vert2(-1), _vert3(-1) ,
			  _texcoord1(-1), _texcoord2(-1), _texcoord3(-1) ,
			  _mesh(0), bActive(FALSE), _index(-1) {};

		  UGSimplificationTriangle(UGint v1, UGint v2, UGint v3) : 
			  _vert1(v1), _vert2(v2), _vert3(v3), _texcoord1(v1), _texcoord2(v2), _texcoord3(v3), _mesh(0), bActive(TRUE),
			  _index(-1) {};

		  UGSimplificationTriangle(UGSimplificationMesh* mp, UGint v1, UGint v2, UGint v3) : 
		  _vert1(v1), _vert2(v2), _vert3(v3), _texcoord1(v1), _texcoord2(v2), _texcoord3(v3), _mesh(mp), bActive(TRUE),
			  _index(-1)
		  {
			  assert(mp);

			  calcNormal();

		  };
		  // copy ctor
		  UGSimplificationTriangle(const UGSimplificationTriangle& t) :  
		  _vert1(t._vert1), _vert2(t._vert2), _vert3(t._vert3), 
		  _texcoord1(t._texcoord1), _texcoord2(t._texcoord2), _texcoord3(t._texcoord3), 
			  _mesh(t._mesh), bActive(t.bActive),
			  _index(t._index)
		  {
			  calcNormal();
		  };

		  // assignment operator
		  UGSimplificationTriangle& operator=(const UGSimplificationTriangle& t)
		  {
			  if (&t == this) return *this; // check for assignment to self
			  _vert1 = t._vert1;
			  _vert2 = t._vert2;
			  _vert3 = t._vert3;
			  _texcoord1 = t._texcoord1;
			  _texcoord2 = t._texcoord2;
			  _texcoord3 = t._texcoord3;
			  _mesh = t._mesh;
			  _normal = t._normal;
			  bActive = t.bActive;
			  _index = t._index;
			  return *this;
		  }

		  // assumes pointing to same list of verts
		  UGbool operator==(const UGSimplificationTriangle& t)
		  {
			  return (_vert1 == t._vert1 && 
				  _vert2 == t._vert2 && 
				  _vert3 == t._vert3 &&
				  _mesh == t._mesh
				  );
		  }

		  // reset the normal for the triangle
		  void calcNormal();

		  void changeMesh(UGSimplificationMesh* mp) {_mesh = mp;};

		  // if the triangle is not active, we set a flag.
		  UGbool isActive() const {return bActive;};
		  void setActive(UGbool b) {bActive = b;};

		  UGbool hasVertex(UGint vi) {
			  return	(vi == _vert1 ||
				  vi == _vert2 ||
				  vi == _vert3);}

		  // When we collapse an edge, we may change the 
		  // vertex of a triangle.
		  void changeVertex(UGint vFrom, UGint vTo)
		  {
			  assert(vFrom != vTo);
			  assert(vFrom == _vert1 || vFrom == _vert2 || vFrom == _vert3);
			  if (vFrom == _vert1) {
				  _vert1 = vTo;
			  } 
			  else if (vFrom == _vert2) {
				  _vert2 = vTo;
			  }
			  else if (vFrom == _vert3) {
				  _vert3 = vTo;
			  }
			  else {
				  //!FIX error
			  }
		  }

		  void getVerts(UGint& v1, UGint& v2, UGint& v3) {v1=_vert1;v2=_vert2;v3=_vert3;}

		  const UGdouble* getVert1();
		  const UGdouble* getVert2();
		  const UGdouble* getVert3();

		  const UGSimplificationVertex& getVert1vertex() const;
		  const UGSimplificationVertex& getVert2vertex() const;
		  const UGSimplificationVertex& getVert3vertex() const;

		  UGdouble* getNormal() {_normArray[0]=_normal.x;
		  _normArray[1]=_normal.y;
		  _normArray[2]=_normal.z;
		  return _normArray;}

		  UGdouble calcArea(); // returns area of triangle

		  UGint getVert1Index() const {return _vert1;}
		  UGint getVert2Index() const {return _vert2;}
		  UGint getVert3Index() const {return _vert3;}
		  UGint getTexcoord1Index() const {return _texcoord1;}
		  UGint getTexcoord2Index() const {return _texcoord2;}
		  UGint getTexcoord3Index() const {return _texcoord3;}

		  const UGVector3d& getNormalVec3() const {return _normal;}

		  UGint getIndex() const {return _index;}
		  void setIndex(UGint i) {_index = i;}

		  // 'd' is from the plane equation ax + by + cz + d = 0
		  UGdouble getD() const {return _d;}

	public:

		UGint _vert1;
		UGint _vert2;
		UGint _vert3;

		UGint _texcoord1;
		UGint _texcoord2;
		UGint _texcoord3;

		UGVector3d _normal; // normal to plane
		UGdouble _normArray[3];

		UGdouble _d;	// This parameter is the "d" in the
		// plane equation ax + by + cz + d = 0
		// The plane equation of this triangle is used.

		UGbool bActive; // active flag

		UGSimplificationMesh* _mesh;

		UGint _index; // index in list of triangles w/in mesh
	};

	//mesh:
	class BASE3D_API UGSimplificationMesh
	{
	public:
		// Constructors and Destructors
		UGSimplificationMesh() {_numVerts = _numTriangles = 0;};
		UGSimplificationMesh(std::vector<UGVector3d>& pts, std::vector<UGint>& triIndexs);
		UGSimplificationMesh(std::vector<UGVector4d>& pts, std::vector<UGint>& triIndexs);
		UGSimplificationMesh(std::vector<UGVector4d>& pts, std::vector<UGint>& triIndexs, std::vector<UGint>& texcoordIndexs);
		~UGSimplificationMesh();

		UGSimplificationMesh(const UGSimplificationMesh&); // copy ctor
		UGSimplificationMesh& operator=(const UGSimplificationMesh&); // assignment op

		// Get list of vertices, triangles
		UGSimplificationVertex& getVertex(UGint index) {return _vlist[index];};
		const UGSimplificationVertex& getVertex(UGint index) const {return _vlist[index];};
		UGSimplificationTriangle& getTri(UGint index) {return _plist[index];};
		const UGSimplificationTriangle& getTri(UGint index) const {return _plist[index];};

		UGint getNumVerts() {return _numVerts;};
		void setNumVerts(UGint n) {_numVerts = n;};
		UGint getNumTriangles() {return _numTriangles;};
		void setNumTriangles(UGint n) {_numTriangles = n;};

		void Normalize();// center mesh around the origin & shrink to fit in [-1, 1]

//		void calcOneVertNormal(UGuint vert); // recalc normal for one vertex

	private:
		vector<UGSimplificationVertex> _vlist;   // list of vertices in mesh
		vector<UGSimplificationTriangle> _plist; // list of triangles in mesh

		UGint _numVerts;
		UGint _numTriangles;

		UGbool operator==(const UGSimplificationMesh&); // don't allow op== -- too expensive

		// get bounding box for mesh
		void setMinMax(UGdouble min[3], UGdouble max[3]);

// 		void calcAllQMatrices(UGSimplificationMesh& mesh); // used for Quadrics method

//		void calcVertNormals(); // Calculate the vertex normals after loading the mesh

	};

	struct EdgeCollapse
	{
		UGint _vfrom;
		UGint _vto;
		set<UGint> _trisRemoved;
		set<UGint> _trisAffected;


		// Used for debugging
		void dumpEdgeCollapse()
		{
// 			std::cout << "**** Edge Collapse Dump ****" << std::endl;
// 
// 			std::cout << "\tFrom Vert# " << _vfrom << " to Vert# " << _vto << std::endl;
// 			cout << "\tTris removed:";
// 			set<int>::iterator pos;
// 			for (pos = _trisRemoved.begin(); pos != _trisRemoved.end(); ++pos) 
// 			{
// 				std::cout << " " << *pos;
// 			}
// 			cout << std::endl << "\tTris affected:";
// 			for (pos = _trisAffected.begin(); pos != _trisAffected.end(); ++pos) 
// 			{
// 				std::cout << " " << *pos;
// 			}
// 			std::cout  << std::endl << "**** End of Edge Collapse Dump ****" << std::endl;
		}
	};

	struct vertexPtr
	{
		UGSimplificationMesh* _meshptr;
		UGint _index; 

		UGbool operator<(const vertexPtr& vp) const 
		{
			return (_meshptr->getVertex(_index) < vp._meshptr->getVertex(vp._index));
		}
	};
	typedef std::multiset<vertexPtr, less<vertexPtr> > vertexPtrSet;
	
	class BASE3D_API UGSimplificationPMesh
	{
	public:
		// Type of progress mesh algorithm
		enum EdgeCost {SHORTEST, MELAX, QUADRIC, QUADRICTRI, MAX_EDGECOST};

		UGSimplificationPMesh(UGSimplificationMesh* mesh, EdgeCost ec, UGfloat fErrorCost = 5.0f, UGbool bReserveBorder = FALSE);

		// Collapse one vertex to another.
		UGbool collapseEdge();

		// One vertex will be split into two vertices -- this
		// is the opposite of a collapse
		UGbool splitVertex();

		// number of edge collapses
		UGint numCollapses() {return _edgeCollList.size();}
		UGint numEdgeCollapses() {return _edgeCollList.size();}

		// number of triangles, and visible triangles in mesh
		UGint numTris() {return _newmesh.getNumTriangles();}
		UGint numVisTris() {return _nVisTriangles;}
		UGSimplificationMesh& GetNewMesh(){return _newmesh;}
		UGbool getTri(UGint i, UGSimplificationTriangle& t) {
			t = _newmesh.getTri(i);
			return TRUE;
		}

//		void UpdateNormal();
	private:

		UGSimplificationMesh* _mesh; // original mesh - not changed
		UGSimplificationMesh _newmesh; // we change this one

		// 边界值
		UGfloat m_fErrorCost;

		EdgeCost _cost; // Type of progressive mesh algorithm

		std::list<EdgeCollapse> _edgeCollList; // list of edge collapses
		std::list<EdgeCollapse>::iterator _edgeCollapseIter;

		//! \brief 是否保留轮廓边界
		UGbool m_bReserveBorder;

//		void UpdateNormalOneVertex(UGint index);
		// functions used to calculate edge collapse costs.  Different
		// methods can be used, depending on user preference.
		UGdouble shortEdgeCollapseCost(UGSimplificationMesh& m, UGSimplificationVertex& v);
// 		UGdouble melaxCollapseCost(UGSimplificationMesh& m, UGSimplificationVertex& v);
// 		UGdouble quadricCollapseCost(UGSimplificationMesh& m, UGSimplificationVertex& v);

		UGint _nVisTriangles; // # of triangles, after we collapse edges

		// Create the list of the edge collapses used
		// to simplify the mesh.
		void createEdgeCollapseList();

		// Used in the QEM edge collapse methods.
// 		void calcAllQMatrices(UGSimplificationMesh& mesh, UGbool bUseTriArea); // used for quadric method
// 		UGdouble calcQuadricError(UGdouble Qsum[4][4], UGSimplificationVertex& v, UGdouble triArea); // used for quadric method

		enum {BOUNDARY_WEIGHT = 1000}; // used to weight border edges so they don't collapse
// 		void applyBorderPenalties(std::set<border> &borderSet, UGSimplificationMesh &mesh);

		UGSimplificationPMesh(const UGSimplificationPMesh&); // don't allow copy ctor -- too expensive
		UGSimplificationPMesh& operator=(const UGSimplificationPMesh&); // don't allow assignment op.
		UGbool operator==(const UGSimplificationPMesh&); // don't allow op==
		// helper function for edge collapse costs
		void calcEdgeCollapseCosts(vertexPtrSet &vertSet, std::vector<vertexPtrSet::iterator> &vertSetVec, 
			UGint nVerts, UGSimplificationMesh &mesh, EdgeCost &cost);

		// Calculate the QEM matrices used to computer edge
		// collapse costs.
// 		void calcQuadricMatrices(EdgeCost &cost, UGSimplificationMesh &mesh);

		// We can't collapse Vertex1 to Vertex2 if Vertex2 is invalid.
		// This can happen if Vertex2 was previously collapsed to a
		// separate vertex.
		void insureEdgeCollapseValid(EdgeCollapse &ec, UGSimplificationVertex &vc, UGSimplificationMesh &mesh, 
			const EdgeCost &cost, UGbool &bBadVertex);

		// Calculate the QEM for the "to vertex" in the edge collapse.
//		void setToVertexQuadric(UGSimplificationVertex &to, UGSimplificationVertex &from, const EdgeCost &cost);

		// At this point, we have an edge collapse.  We're collapsing the "from vertex"
		// to the "to vertex."  For all the surrounding triangles which use this edge, 
		// update "from vertex" to the "to vertex".  Also keep track of the vertices
		// in the surrounding triangles. 
		void updateTriangles(EdgeCollapse &ec, UGSimplificationVertex &vc, std::set<UGint> &affectedVerts, UGSimplificationMesh &mesh);


		// These affected vertices are not in the current collapse, 
		// but are in the triangles which share the collapsed edge.
		void updateAffectedVertNeighbors(UGSimplificationVertex &vert, const EdgeCollapse &ec, 
			std::set<UGint> &affectedVerts);

		// Reset the edge collapse costs of vertices which were
		// affected by a previous edge collapse.
		void resetAffectedVertCosts(const EdgeCost &cost, UGSimplificationMesh &newmesh, UGSimplificationVertex &vert);

		// If this vertex has no active triangles (i.e. triangles which have
		// not been removed from the mesh) then set it to inactive.
		void removeVertIfNecessary(UGSimplificationVertex &vert, vertexPtrSet &vertSet, 
			std::vector<vertexPtrSet::iterator> &vertSetVec, 
			UGSimplificationMesh &mesh, const EdgeCost &cost, 
			std::set<UGint> &affectedQuadricVerts);

		// Update the vertices affected by the most recent edge collapse
		void updateAffectedVerts(UGSimplificationMesh &_newmesh, std::vector<vertexPtrSet::iterator> &vertSetVec, 
			vertexPtrSet &vertSet, const EdgeCollapse &ec, 
			std::set<UGint> &affectedVerts, const EdgeCost &cost, 
			std::set<UGint> &affectedQuadricVerts);

		// Recalculate the QEM matrices (yeah, that's redundant) if we're
		// using the Quadrics to calculate edge collapse costs.
// 		void recalcQuadricCollapseCosts(std::set<UGint> &affectedQuadricVerts, 
//			UGSimplificationMesh &mesh, const EdgeCost &cost);

		// Calculate the list of edge collapses.  Each edge collapse
		// consists of two vertices:  a "from vertex" and a "to vertex".
		// The "from vertex" is collapsed to the "to vertex".  The
		// "from vertex" is removed from the mesh.
		void buildEdgeCollapseList(UGSimplificationMesh &mesh, const EdgeCost &cost, 
			std::list<EdgeCollapse> &_edgeCollList,
			vertexPtrSet &vertSet, 
			std::vector<vertexPtrSet::iterator> &vertSetVec);

		// Helper function for melaxCollapseCost().  This function
		// will loop through all the triangles to which this vertex
		// belongs.
		void calcMelaxMaxValue(UGSimplificationMesh &mesh, std::set<UGint> &adjfaces, 
			UGSimplificationVertex &v, std::set<UGint> &tneighbors,
			UGdouble &retmaxValue, 
			UGbool &bMaxValueFound);
	};

}
#endif
