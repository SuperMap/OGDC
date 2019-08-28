//
//  Author:  Peter Bonsma
//  $Date: 1999-12-31 23:59:59 +0000 (Wed, 31 Jan 1999) $
//  $Revision: 3999 $
//  Project: IFC Engine DLL
//
//  In case of use of the DLL:
//  be aware of license fee for use of this DLL when used commercially
//  more info for commercial use:  peter.bonsma@rdf.bg
//

#ifndef __RDF_LTD__IFCENGINE_H
#define __RDF_LTD__IFCENGINE_H


#include "engdef.h"


#ifdef _WINDOWS
	#ifdef _USRDLL
		#define DECL //__declspec(dllexport)
		// The use of the Microsoft-specific
		// __declspec(dllexport)
		// is needed only when
		// not using a .def file.
	#else
		#define DECL __declspec(dllimport) 
	#endif
	#define STDC __stdcall
#else
	#define DECL /*nothing*/
	#define STDC /*nothing*/
#endif


#ifdef __cplusplus
	extern "C" {
#endif


#define		sdaiADB					1
#define		sdaiAGGR				sdaiADB + 1
#define		sdaiBINARY				sdaiAGGR + 1
#define		sdaiBOOLEAN				sdaiBINARY + 1
#define		sdaiENUM				sdaiBOOLEAN + 1
#define		sdaiINSTANCE			sdaiENUM + 1
#define		sdaiINTEGER				sdaiINSTANCE + 1
#define		sdaiLOGICAL				sdaiINTEGER + 1
#define		sdaiREAL				sdaiLOGICAL + 1
#define		sdaiSTRING				sdaiREAL + 1
#define		sdaiUNICODE				sdaiSTRING + 1
#define		sdaiEXPRESSSTRING		sdaiUNICODE + 1
#define		engiGLOBALID			sdaiEXPRESSSTRING + 1

typedef void(*LOGCB)(const char *);

/*	Note on sdaiSTRING and sdaiUNICODE

	sdaiUNICODE
		this will convert all internal strings from/too unicode, the internal representation and what is written to the IFC file is mapped
			"\" will be converted into "\\" to enable basic parses to still interpret file paths
			"'" will be converted to \X1\hh\X0\ or \X2\00hh\X0\ to prevent basic interpreters to read files with strings containing these characters

	sdaiSTRING
		this will leave all information as is in the IFC file, the rules are that char's ( int ) 32 to 126 (inclusive) will be kept
		all other strings will be converted to \X1\hh\X0\ or \X2\00hh\X0\		*/


#define		sdaiARRAY				1
#define		sdaiLIST				2
#define		sdaiSET					3
#define		sdaiBAG					4

typedef	int_t			* SdaiAggr;
typedef	void			* SdaiAttr;
typedef	int_t			SdaiInstance;
typedef	int_t			SdaiInteger;
typedef	int_t			SdaiModel;
typedef	int_t			SdaiPrimitiveType;
typedef	int_t			* SdaiSet;
typedef	char			* SdaiString;



//
//  Calls for File IO
//
void	DECL STDC	sdaiCloseModel(
										int_t		model
									);

int_t	DECL STDC	sdaiCreateModelBN(
										int_t		repository,
										char		* fileName,
										char		* schemaName
									);

int_t	DECL STDC	sdaiCreateModelBNUnicode(
										int_t		repository,
										void		* fileName,
										void		* schemaName
									);

int_t	DECL STDC	sdaiOpenModelBN(
										int_t		repository,
										char		* fileName,
										char		* schemaName
									);

int_t	DECL STDC	sdaiOpenModelBNUnicode(
										int_t		repository,
										void		* fileName,
										void		* schemaName
									);

void	DECL STDC	sdaiSaveModelBN(
										int_t		model,
										char		* fileName
									);

void	DECL STDC	sdaiSaveModelBNUnicode(
										int_t		model,
										void		* fileName
									);

void	DECL STDC	sdaiSaveModelAsXmlBN(
										int_t		model,
										char		* fileName
									);

void	DECL STDC	sdaiSaveModelAsXmlBNUnicode(
										int_t		model,
										void		* fileName
									);

void	DECL STDC	sdaiSaveModelAsSimpleXmlBN(
										int_t		model,
										char		* fileName
									);

void	DECL STDC	sdaiSaveModelAsSimpleXmlBNUnicode(
										int_t		model,
										void		* fileName
									);

//
//  Schema Reading
//
int_t	DECL STDC	sdaiGetEntity(
										int_t		model,
										char		* entityName
									);

void	DECL STDC	engiGetEnumerationValue(
										int_t		attribute,
										int_t		index,
										int_t		valueType,
										char		** enumerationValue
									);

int_t	DECL STDC	engiGetEntityArgument(
										int_t		entity, 
										char		* argumentName
									);

int_t	DECL STDC	engiGetEntityArgumentIndex(
										int_t		entity, 
										char		* argumentName
									);

void	DECL STDC	engiGetEntityArgumentName(
										int_t		entity,
										int_t		index,
										int_t		valueType,
										char		** argumentName
									);

void	DECL STDC	engiGetEntityArgumentType(
										int_t		entity,
										int_t		index,
										int_t		* argumentType
									);

int_t	DECL STDC	engiGetEntityCount(
										int_t		model
									);

int_t	DECL STDC	engiGetEntityElement(
										int_t		model,
										int_t		index
									);

int_t	DECL * STDC	sdaiGetEntityExtent(
										int_t		model,
										int_t		entity
									);

int_t	DECL * STDC	sdaiGetEntityExtentBN(
										int_t		model,
										char		* entityName
									);

void	DECL STDC	engiGetEntityName(
										int_t		entity,
										int_t		valueType,
										char		** entityName
									);

int_t	DECL STDC	engiGetEntityNoArguments(
										int_t		entity
									);

int_t	DECL STDC	engiGetEntityParent(
										int_t		entity
									);

int_t	DECL STDC	engiGetAttrOptionalBN(
										int_t		entity,
										char		* attributeName
									);

//
//  Instance Header
//
int_t	DECL STDC	GetSPFFHeaderItem(
										int_t		model,
										int_t		itemIndex,
										int_t		itemSubIndex,
										int_t		valueType,
										char		** value
									);

void	DECL STDC	SetSPFFHeader(
										int_t		model,
										char		* description,
										char		* implementationLevel,
										char		* name,
										char		* timeStamp,
										char		* author,
										char		* organization,
										char		* preprocessorVersion,
										char		* originatingSystem,
										char		* authorization,
										char		* fileSchema
									);

int_t	DECL STDC	SetSPFFHeaderItem(
										int_t		model,
										int_t		itemIndex,
										int_t		itemSubIndex,
										int_t		valueType,
										char		* value
									);

//
//  Instance Reading
//
int_t	DECL STDC	sdaiGetADBType(
										void		* ADB
									);

char	DECL * STDC	sdaiGetADBTypePath(
										void		* ADB,
										int_t		typeNameNumber
									);

void	DECL STDC	sdaiGetADBTypePathx(
										void		* ADB,
										int_t		typeNameNumber,
										char		** result
									);

void	DECL STDC	sdaiGetADBValue(
										void		* ADB,
										int_t		valueType,
										void		* value
									);

void	DECL * STDC	engiGetAggrElement(
										int_t		* aggregate,
										int_t		elementIndex,
										int_t		valueType,
										void		* value
									);

void	DECL STDC	engiGetAggrType(
										int_t		* aggregate,
										int_t		* aggragateType
									);

void	DECL STDC	engiGetAggrTypex(
										int_t		* aggregate,
										int_t		* aggragateType
									);

void	DECL * STDC	sdaiGetAttr(
										int_t		instance,
										void		* attribute,
										int_t		valueType,
										void		* value
									);

void	DECL * STDC	sdaiGetAttrBN(
										int_t		instance,
										char		* attributeName,
										int_t		valueType,
										void		* value
									);

void	DECL * STDC	sdaiGetAttrDefinition(
										int_t		entity,
										char		* attributeName
									);

int_t	DECL STDC	sdaiGetInstanceType(
										int_t		instance
									);

int_t	DECL STDC	sdaiGetMemberCount(
										int_t		* aggregate
									);

int_t	DECL STDC	sdaiIsKindOf(
										int_t		instance,
										int_t		entity
									);

//
//  Instance Writing
//
void	DECL STDC	sdaiAppend(
										int_t		list,
										int_t		valueType,
										void		* value
									);

void	DECL * STDC	sdaiCreateADB(
										int_t		valueType,
										void		* value
									);

int_t	DECL * STDC	sdaiCreateAggr(
										int_t		instance,
										void		* attribute
									);

int_t	DECL * STDC	sdaiCreateAggrBN(
										int_t		instance,
										char		* attributeName
									);

int_t	DECL * STDC	sdaiCreateNestedAggr(
										int_t		* aggr
									);

int_t	DECL STDC	sdaiCreateInstance(
										int_t		model,
										int_t		entity
									);

int_t	DECL STDC	sdaiCreateInstanceBN(
										int_t		model,
										char		* entityName
									);

void	DECL STDC	sdaiDeleteInstance(
										int_t		instance
									);

void	DECL STDC	sdaiPutADBTypePath(
										void		* ADB,
										int_t		pathCount,
										char		* path
									);

void	DECL STDC	sdaiPutAttr(
										int_t		instance,
										void		* attribute,
										int_t		valueType,
										void		* value
									);

void	DECL STDC	sdaiPutAttrBN(
										int_t		instance,
										char		* attributeName,
										int_t		valueType,
										void		* value
									);

void	DECL STDC	engiSetComment(
										int_t		instance,
										char		* comment
									);

//
//  Controling Calls
//
void	DECL STDC	circleSegments(
										int_t		circles,
										int_t		smallCircles
									);

void	DECL STDC	cleanMemory(
										int_t		model,
										int_t		mode
									);

void	DECL STDC	internalGetXMLID(
										int_t		instance,
										char		** XMLID
									);

int_t	DECL STDC	internalGetP21Line(
										int_t		instance
									);

int_t	DECL STDC	internalGetInstanceFromP21Line(
										int_t		model,
										int_t		P21Line
									);

int_t	DECL STDC	setStringUnicode(
										int_t		unicode
									);

//
//  Geometry Interaction
//
int_t	DECL STDC	initializeModellingInstance(
										int_t		model,
										int_t		* noVertices,
										int_t		* noIndices,
										double		scale,
										int_t		instance
									);

int_t	DECL STDC	finalizeModelling(
										int_t		model,
										float		* vertices,
										int_t		* indices,
										int_t		FVF
									);

int_t	DECL STDC	getInstanceInModelling(
										int_t		model,
										int_t		instance,
										int_t		mode,
										int_t		* startVertex,
										int_t		* startIndex,
										int_t		* primitiveCount
									);

void	DECL STDC	setVertexOffset(
										int_t		model,
										double		x,
										double		y,
										double		z
									);

void	DECL STDC	setFilter(
										int_t		model,
										int_t		setting,
										int_t		mask
									);

void	DECL STDC	setFormat(
										int_t		model,
										int_t		setting,
										int_t		mask
									);

int_t	DECL STDC	getConceptualFaceCnt(
										int_t		instance
									);

int_t	DECL STDC	getConceptualFaceEx(
										int_t		instance,
										int_t		index,
										int_t		* startIndexTriangles,
										int_t		* noIndicesTriangles,
										int_t		* startIndexLines,
										int_t		* noIndicesLines,
										int_t		* startIndexPoints,
										int_t		* noIndicesPoints,
										int_t		* startIndexFacesPolygons,
										int_t		* noIndicesFacesPolygons,
										int_t		* startIndexConceptualFacePolygons,
										int_t		* noIndicesConceptualFacePolygons
									);

//
//  Not documented API calls
//
int_t	DECL * STDC	xxxxGetEntityAndSubTypesExtent(
										int_t		model,
										int_t		entity
									);

int_t	DECL * STDC	xxxxGetEntityAndSubTypesExtentBN(
										int_t		model,
										char		* entityName
									);

int_t	DECL * STDC	xxxxGetInstancesUsing(
										int_t		instance
									);

int_t	DECL STDC	xxxxDeleteFromAggregation(
										int_t		instance,
										int_t		* aggregate,
										int_t		elementIndex
									);

void	DECL * STDC	xxxxGetAttrDefinitionByValue(
										int_t		instance,
										void		* value
									);

void	DECL STDC	xxxxGetAttrNameByIndex(
										int_t		instance,
										int_t		index,
										char		** name
									);

int_t	DECL STDC	xxxxOpenModelByStream(
										int_t		repository,
										void		* callback,
										char		* schemaName
									);

void	DECL * STDC	engiGetAggrElement_I(
										int_t		* aggregate,
										int_t		elementIndex,
										int_t		valueType,
										void		* value
									);

int_t	DECL STDC	engiGetInstanceMetaInfo(
										int_t		* instance,
										int_t		* localId,
										char		** entityName,
										char		** entityNameUC
									);

void	DECL STDC	engiGetEntityProperty(
										int_t		entity,
										int_t		index,
										char		** propertyName,
										int_t		* optional,
										int_t		* type,
										int_t		* _array,
										int_t		* set,
										int_t		* list,
										int_t		* bag,
										int_t		* min,
										int_t		* max,
										int_t		* referenceEntity,
										int_t		* inverse
									);

int_t	DECL STDC	engiGetInstanceLocalId(
										int_t		instance
									);

void	DECL STDC	engiGetInstanceClassInfoEx(
										int_t		instance,
										void		** classInfo
									);

char	DECL * STDC	engiGetInstanceClassInfo(
										int_t		instance
									);

char	DECL * STDC	engiGetInstanceClassInfoUC(
										int_t		instance
									);

void	DECL STDC	sdaiBeginning(
										int_t		iterator
									);

int_t	DECL STDC	sdaiCreateInstanceEI(
										int_t		model,
										int_t		entity,
										int_t		express_id
									);

int_t	DECL STDC	sdaiCreateInstanceBNEI(
										int_t		model,
										char		* entityName,
										int_t		express_id
									);

void	DECL * STDC	sdaiCreateIterator(
										int_t		* aggregate
									);

void	DECL STDC	sdaiDeleteIterator(
										int_t		iterator
									);

void	DECL STDC	sdaiEnd(
										int_t		iterator
									);

int_t	DECL STDC	sdaiErrorQuery(
									);

void	DECL * STDC	sdaiGetAggrByIterator(
										int_t		iterator,
										int_t		valueType,
										void		* value
									);

int_t	DECL STDC	sdaiIsInstanceOf(
										int_t		instance,
										int_t		entity
									);

int_t	DECL STDC	sdaiIsInstanceOfBN(
										int_t		instance,
										char		* entityName
									);

int_t	DECL STDC	sdaiNext(
										int_t		iterator
									);

void	DECL STDC	sdaiPrepend(
										int_t		list,
										int_t		valueType,
										void		* value
									);

int_t	DECL STDC	sdaiPrevious(
										int_t		iterator
									);

void	DECL STDC	sdaiPutAggrByIterator(
										int_t		iterator,
										int_t		valueType,
										void		* value
									);

int_t	DECL STDC	sdaiTestAttr(
										int_t		instance,
										void		* attribute
									);

int_t	DECL STDC	sdaiTestAttrBN(
										int_t		instance,
										char		* attributeName
									);

int_t	DECL STDC	engiGetAttrType(
										int_t		instance,
										void		* attribute
									);

int_t	DECL STDC	engiGetAttrTypeBN(
										int_t		instance,
										char		* attributeName
									);

int_t	DECL STDC	initializeModellingInstanceEx(
										int_t		model,
										int_t		* noVertices,
										int_t		* noIndices,
										double		scale,
										int_t		instance,
										int_t		instanceList
									);

void	DECL STDC	exportModellingAsOWL(
										int_t		model,
										char		* fileName
									);

void	DECL STDC	owlGetModel(
										int_t		model,
										int64_t		* owlModel
									);

void	DECL STDC	owlGetInstance(
										int_t		model,
										int_t		instance,
										int64_t		* owlInstance
									);

void	DECL STDC	owlBuildInstance(
										int_t		model,
										int_t		instance,
										int64_t		* owlInstance
									);

void	DECL STDC	owlBuildInstances(
										int_t		model,
										int_t		instance,
										int64_t		* owlInstanceComplete,
										int64_t		* owlInstanceSolids,
										int64_t		* owlInstanceVoids
									);

void	DECL STDC	owlGetMappedItem(
										int_t		model,
										int_t		instance,
										int64_t		* owlInstance,
										double		* transformationMatrix
									);

int_t	DECL STDC	getInstanceDerivedPropertiesInModelling(
										int_t		model,
										int_t		instance,
										double		* height,
										double		* width,
										double		* thickness
									);

int_t	DECL STDC	getInstanceDerivedBoundingBox(
										int_t		model,
										int_t		instance,
										double		* Ox,
										double		* Oy,
										double		* Oz,
										double		* Vx,
										double		* Vy,
										double		* Vz
									);

int_t	DECL STDC getInstanceTransformationMatrix(
										int_t		model,
										int_t		instance,
										double		* _11,
										double		* _12,
										double		* _13,
										double		* _14,
										double		* _21,
										double		* _22,
										double		* _23,
										double		* _24,
										double		* _31,
										double		* _32,
										double		* _33,
										double		* _34,
										double		* _41,
										double		* _42,
										double		* _43,
										double		* _44
									);

int_t	DECL STDC	getInstanceDerivedTransformationMatrix(
										int_t		model,
										int_t		instance,
										double		* _11,
										double		* _12,
										double		* _13,
										double		* _14,
										double		* _21,
										double		* _22,
										double		* _23,
										double		* _24,
										double		* _31,
										double		* _32,
										double		* _33,
										double		* _34,
										double		* _41,
										double		* _42,
										double		* _43,
										double		* _44
									);

int_t	DECL STDC	getTimeStamp(
										int_t		model
									);

char	DECL * STDC	getChangedData(
										int_t		model,
										int_t		* timeStamp
									);

void	DECL STDC	setChangedData(
										int_t		model,
										int_t		* timeStamp,
										char		* changedData
									);

void	DECL * STDC	internalGetBoundingBox(
										int_t		model,
										int_t		instance
									);

void	DECL * STDC	internalGetCenter(
										int_t		model,
										int_t		instance
									);

void	DECL STDC	internalSetLink(
										int_t		instance,
										char		* attributeName,
										int_t		linked_id
									);

void	DECL STDC	internalAddAggrLink(
										int_t		list,
										int_t		linked_id
									);

char	DECL * STDC	sdaiGetStringAttrBN(
										int_t		instance,
										char		* attributeName
									);

int_t	DECL STDC	sdaiGetInstanceAttrBN(
										int_t		instance,
										char		* attributeName
									);

int_t	DECL STDC	sdaiGetAggregationAttrBN(
										int_t		instance,
										char		* attributeName
									);

void	DECL STDC	engiGetNotReferedAggr(
										int_t		model,
										int_t		* value
									);

void	DECL STDC	engiGetAttributeAggr(
										int_t		instance,
										int_t		* value
									);

void	DECL STDC	engiGetAggrUnknownElement(
										int_t		* aggregate,
										int_t		elementIndex,
										int_t		* valueType,
										void		* value
									);

int_t	DECL * STDC	sdaiFindInstanceUsers(
										int_t		instance,
										int_t		* domain,
										int_t		* resultList
									);

int_t	DECL * STDC	sdaiFindInstanceUsedIn(
										int_t		instance,
										int_t		* domain,
										int_t		* resultList
									);

int_t	DECL STDC	sdaiplusGetAggregationType(
										int_t		instance,
										int_t		* aggregation
									);

int_t	DECL STDC	iterateOverInstances(
										int_t		model,
										int_t		instance,
										int_t		* entity,
										void		* entityName
									);

int_t	DECL STDC	iterateOverProperties(
										int_t		entity,
										int_t		index
									);

void	DECL STDC	createGeometryConversion(
										int_t		instance,
										int64_t		* owlInstance
									);

void	DECL STDC	convertInstance(
										int_t		instance
									);

void	DECL STDC	setRestriction(
										int_t		instance
									);

void	DECL STDC	setMaximumSegmentationLength(
										int_t		model,
										double		length
									);

void	DECL STDC	setBRepProperties(
										int_t		model,
										int64_t		consistancyCheck,
										double		fraction,
										double		epsilon,
										int_t		maxVerticesSize
									);

#ifdef __cplusplus
	}
#endif

#undef DECL
#undef STDC
#endif
