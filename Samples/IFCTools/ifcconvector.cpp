#include "ifcconvector.h"
#include "IFCEngine/engine.h"
#include "Stream/UGFile.h"

#include "OGDC/OgdcDataSource.h"
#include "OGDC/OgdcProviderManager.h"

#include <QMessageBox>
#include <QProgressDialog>
#include "Base3D/UGReindexer.h"
#include "Base3D/UGModelEM.h"
#include "Base3D/UGModelNodeShells.h"
#include "Base3D/UGModelNode.h"
#include "Base3D/UGVector3d.h"

#include "Geometry3D/UGGeoModelPro.h"


static int_t flagbit0 = 1;          // 2^^0    0000.0000..0000.0001
static int_t flagbit1 = 2;          // 2^^1    0000.0000..0000.0010
static int_t flagbit2 = 4;          // 2^^2    0000.0000..0000.0100
static int_t flagbit3 = 8;          // 2^^3    0000.0000..0000.1000

static int_t flagbit4 = 16;         // 2^^4    0000.0000..0001.0000
static int_t flagbit5 = 32;         // 2^^5    0000.0000..0010.0000
static int_t flagbit6 = 64;         // 2^^6    0000.0000..0100.0000
static int_t flagbit7 = 128;        // 2^^7    0000.0000..1000.0000

static int_t flagbit8 = 256;        // 2^^8    0000.0001..0000.0000
static int_t flagbit9 = 512;        // 2^^9    0000.0010..0000.0000
static int_t flagbit10 = 1024;       // 2^^10   0000.0100..0000.0000
static int_t flagbit11 = 2048;       // 2^^11   0000.1000..0000.0000

static int_t flagbit12 = 4096;       // 2^^12   0001.0000..0000.0000
static int_t flagbit13 = 8192;       // 2^^13   0010.0000..0000.0000
static int_t flagbit14 = 16384;      // 2^^14   0100.0000..0000.0000
static int_t flagbit15 = 32768;      // 2^^15   1000.0000..0000.0000

static bool	contains(wchar_t * txtI, wchar_t * txtII)
{
	int_t	i = 0;
	while (txtI[i] && txtII[i]) {
		if (txtI[i] != txtII[i]) {
			return	false;
		}
		i++;
	}
	if (txtII[i]) {
		return	false;
	}
	else {
		return	true;
	}
}
static bool	__equals(
	wchar_t		* txtI,
	wchar_t		* txtII
	)
{
	int_t i = 0;
	if (txtI && txtII) {
		while (txtI[i]) {
			if (txtI[i] != txtII[i]) {
				return	false;
			}
			i++;
		}
		if (txtII[i]) {
			return	false;
		}
	}
	else if (txtI || txtII) {
		return	false;
	}
	return	true;
}

static bool	equalStr(wchar_t * txtI, wchar_t * txtII)
{
	int_t i = 0;
	if (txtI && txtII) {
		while (txtI[i]) {
			if (txtI[i] != txtII[i]) {
				return	false;
			}
			i++;
		}
		if (txtII[i]) {
			return	false;
		}
	}
	else if (txtI || txtII) {
		return	false;
	}
	return	true;
}

static wchar_t	* copyStr(wchar_t * txt)
{
	if (txt) {
		int_t	i = 0;
		while (txt[i]) { i++; }
		wchar_t	* rValue = new wchar_t[i + 1];
		i = 0;
		while (txt[i]) {
			rValue[i] = txt[i];
			i++;
		}
		rValue[i] = 0;

		return	rValue;
	}
	else {
		return	0;
	}
}

IFCConvector::IFCConvector(QObject *parent)
	: QObject(parent)
{
	m_ifcSchemaName_IFC2x3 = NULL;
	m_ifcSchemaName_IFC4 = NULL;
	m_ifcSchemaName_IFC4x1 = NULL;

	m_ifcObjects = NULL;
	m_firstFreeIfcObject = &m_ifcObjects;

	m_ifcModel = NULL;

	m_defaultMaterial = NULL;
	m_firstMaterial = NULL;
	m_lastMaterial = NULL;

	m_units = NULL;

	m_bIsPlaceSphere = true;

	Init();
}

IFCConvector::~IFCConvector()
{
	Clear();
}

void IFCConvector::Init()
{
	// 载入配置文件
	UGString cfilePath = UGFile::GetAppPath() + _U("config/");

	wchar_t* configFilePath = (wchar_t*) cfilePath.Cstr();

	int_t i = wcslen(configFilePath);

	m_ifcSchemaName_IFC2x3 = new wchar_t[i + wcslen(L"IFC2X3_TC1.exp") + 1];
	memcpy(&m_ifcSchemaName_IFC2x3[0], configFilePath, i * sizeof(wchar_t));
	memcpy(&m_ifcSchemaName_IFC2x3[i], L"IFC2X3_TC1.exp", (wcslen(L"IFC2X3_TC1.exp") + 1) * sizeof(wchar_t));

	m_ifcSchemaName_IFC4 = new wchar_t[i + wcslen(L"IFC4_ADD2.exp") + 1];
	memcpy(&m_ifcSchemaName_IFC4[0], configFilePath, i * sizeof(wchar_t));
	memcpy(&m_ifcSchemaName_IFC4[i], L"IFC4_ADD2.exp", (wcslen(L"IFC4_ADD2.exp") + 1) * sizeof(wchar_t));

	m_ifcSchemaName_IFC4x1 = new wchar_t[i + wcslen(L"IFC4X1.exp") + 1];
	memcpy(&m_ifcSchemaName_IFC4x1[0], configFilePath, i * sizeof(wchar_t));
	memcpy(&m_ifcSchemaName_IFC4x1[i], L"IFC4X1.exp", (wcslen(L"IFC4X1.exp") + 1) * sizeof(wchar_t));
}

void IFCConvector::Clear()
{
	delete[] m_ifcSchemaName_IFC2x3;
	delete[] m_ifcSchemaName_IFC4;
	delete[] m_ifcSchemaName_IFC4x1;

	ClearUpIfcModel();

	m_ifcSchemaName_IFC2x3 = NULL;
	m_ifcSchemaName_IFC4 = NULL;
	m_ifcSchemaName_IFC4x1 = NULL;

	m_ifcObjects = NULL;
	m_firstFreeIfcObject = &m_ifcObjects;

	//m_ifcPropertySet = NULL;
	m_ifcModel = NULL;

	m_defaultMaterial = NULL;
	m_firstMaterial = NULL;
	m_lastMaterial = NULL;

	m_units = NULL;
}
//-------------------------------------------------------------------------------------------------------
void IFCConvector::ClearUnits()
{
	while (m_units) {
		IFC__SIUNIT	* unitToRemove = m_units;

		m_units = m_units->next;
		delete unitToRemove;
	}

	m_units = NULL;
}

void IFCConvector::GetUnits(int_t ifcModel)
{
	int_t ifcRelAggregates_TYPE = sdaiGetEntity(ifcModel, (char*)L"IFCRELAGGREGATES");
	int_t ifcRelContainedInSpatialStructure_TYPE = sdaiGetEntity(ifcModel, (char*)L"IFCRELCONTAINEDINSPATIALSTRUCTURE");

	int_t* ifcProjectInstances = sdaiGetEntityExtentBN(ifcModel, (char*)L"IFCPROJECT");
	int_t noIfcProjectInstances = sdaiGetMemberCount(ifcProjectInstances);
	for (int_t i = 0; i < noIfcProjectInstances; ++i) {
		int_t	ifcProjectInstance = 0;
		engiGetAggrElement(ifcProjectInstances, i, sdaiINSTANCE, &ifcProjectInstance);

		m_units = GetUnits(ifcModel, ifcProjectInstance);
	}
}

IFC__SIUNIT* IFCConvector::GetUnits(int_t ifcModel, int_t ifcProjectInstance)
{
	IFC__SIUNIT	* firstUnit = 0;

	int_t	ifcUnitAssignmentInstance = 0;
	sdaiGetAttrBN(ifcProjectInstance, (char*)L"UnitsInContext", sdaiINSTANCE, &ifcUnitAssignmentInstance);

	int_t	ifcConversianBasedUnit_TYPE = sdaiGetEntity(ifcModel, (char*)L"IFCCONVERSIONBASEDUNIT"),
		ifcSIUnit_TYPE = sdaiGetEntity(ifcModel, (char*)L"IFCSIUNIT");

	int_t	* unit_set = 0, unit_cnt, i = 0;

	sdaiGetAttrBN(ifcUnitAssignmentInstance, (char*)L"Units", sdaiAGGR, &unit_set);

	unit_cnt = sdaiGetMemberCount(unit_set);
	for (i = 0; i < unit_cnt; ++i) {
		int_t	ifcUnitInstance = 0;
		engiGetAggrElement(unit_set, i, sdaiINSTANCE, &ifcUnitInstance);
		if (sdaiGetInstanceType(ifcUnitInstance) == ifcConversianBasedUnit_TYPE) {
			IFC__SIUNIT	* unit = new IFC__SIUNIT();
			unit->unitType = 0;
			unit->prefix = 0;
			unit->name = 0;
			unit->next = firstUnit;
			firstUnit = unit;

			int_t	ifcMeasureWithUnitInstance = 0;

			sdaiGetAttrBN(ifcUnitInstance, (char*)L"ConversionFactor", sdaiINSTANCE, &ifcMeasureWithUnitInstance);

			if (ifcMeasureWithUnitInstance) {
				int_t	ifcSIUnitInstance = 0;

				int_t  * adb = 0;
				sdaiGetAttrBN(ifcMeasureWithUnitInstance, (char*)L"UnitComponent", sdaiINSTANCE, &ifcSIUnitInstance);
				sdaiGetAttrBN(ifcMeasureWithUnitInstance, (char*)L"ValueComponent", sdaiADB, &adb);

				double value = 0;
				sdaiGetADBValue(adb, sdaiREAL, &value);

				if (sdaiGetInstanceType(ifcSIUnitInstance) == ifcSIUnit_TYPE) {
					wchar_t	* unitType = 0, *prefix = 0, *name = 0;

					sdaiGetAttrBN(ifcSIUnitInstance, (char*)L"UnitType", sdaiUNICODE, &unitType);
					sdaiGetAttrBN(ifcSIUnitInstance, (char*)L"Prefix", sdaiUNICODE, &prefix);
					sdaiGetAttrBN(ifcSIUnitInstance, (char*)L"Name", sdaiUNICODE, &name);

					UnitAddUnitType(unit, unitType);
					UnitAddPrefix(unit, prefix);
					UnitAddName(unit, name);
				}
				else {
					//ASSERT(false);
				}
			}
			else {
				//ASSERT(false);
			}
		}
		else if (sdaiGetInstanceType(ifcUnitInstance) == ifcSIUnit_TYPE) {
			IFC__SIUNIT	* unit = new IFC__SIUNIT();
			unit->unitType = 0;
			unit->prefix = 0;
			unit->name = 0;
			unit->next = firstUnit;
			firstUnit = unit;

			wchar_t	* unitType = 0, *prefix = 0, *name = 0;

			sdaiGetAttrBN(ifcUnitInstance, (char*)L"UnitType", sdaiUNICODE, &unitType);
			sdaiGetAttrBN(ifcUnitInstance, (char*)L"Prefix", sdaiUNICODE, &prefix);
			sdaiGetAttrBN(ifcUnitInstance, (char*)L"Name", sdaiUNICODE, &name);

			UnitAddUnitType(unit, unitType);
			UnitAddPrefix(unit, prefix);
			UnitAddName(unit, name);
		}
		else {
			///////////////////			ASSERT(false);
		}
	}

	return	firstUnit;
}

wchar_t	* IFCConvector::GetUnit(IFC__SIUNIT * units, wchar_t * unitType)
{
	IFC__SIUNIT	* unit = units;

	while (unit) {
		if (equalStr(unit->unitType, unitType)) {
			int_t i = 0, j = 0;
			if (unit->prefix) {
				while (unit->prefix[i]) { i++; }
				i++;
			}
			if (unit->name) {
				while (unit->name[j]) { j++; }
			}

			wchar_t	* rValue = new wchar_t[i + j + 1];

			i = 0;
			if (unit->prefix) {
				while (unit->prefix[i]) { rValue[i++] = unit->prefix[i]; }
				rValue[i++] = ' ';
			}
			j = 0;
			if (unit->name) {
				while (unit->name[j]) { rValue[i + j++] = unit->name[j]; }
				rValue[i + j] = 0;
			}
			return	rValue;
		}
		unit = unit->next;
	}

	return	0;
}

void IFCConvector::UnitAddUnitType(IFC__SIUNIT * unit, wchar_t * unitType)
{
	//
	//	unitType
	//
	if (equalStr(unitType, L".ABSORBEDDOSEUNIT.")) {
		unit->type = ABSORBEDDOSEUNIT;
		unit->unitType = L"ABSORBEDDOSEUNIT";
	}
	else if (equalStr(unitType, L".AREAUNIT.")) {
		unit->type = AREAUNIT;
		unit->unitType = L"AREAUNIT";
	}
	else if (equalStr(unitType, L".DOSEEQUIVALENTUNIT.")) {
		unit->type = DOSEEQUIVALENTUNIT;
		unit->unitType = L"DOSEEQUIVALENTUNIT";
	}
	else if (equalStr(unitType, L".ELECTRICCAPACITANCEUNIT.")) {
		unit->type = ELECTRICCAPACITANCEUNIT;
		unit->unitType = L"ELECTRICCAPACITANCEUNIT";
	}
	else if (equalStr(unitType, L".ELECTRICCHARGEUNIT.")) {
		unit->type = ELECTRICCHARGEUNIT;
		unit->unitType = L"ELECTRICCHARGEUNIT";
	}
	else if (equalStr(unitType, L".ELECTRICCONDUCTANCEUNIT.")) {
		unit->type = ELECTRICCONDUCTANCEUNIT;
		unit->unitType = L"ELECTRICCONDUCTANCEUNIT";
	}
	else if (equalStr(unitType, L".ELECTRICCURRENTUNIT.")) {
		unit->type = ELECTRICCURRENTUNIT;
		unit->unitType = L"ELECTRICCURRENTUNIT";
	}
	else if (equalStr(unitType, L".ELECTRICRESISTANCEUNIT.")) {
		unit->type = ELECTRICRESISTANCEUNIT;
		unit->unitType = L"ELECTRICRESISTANCEUNIT";
	}
	else if (equalStr(unitType, L".ELECTRICVOLTAGEUNIT.")) {
		unit->type = ELECTRICVOLTAGEUNIT;
		unit->unitType = L"ELECTRICVOLTAGEUNIT";
	}
	else if (equalStr(unitType, L".ENERGYUNIT.")) {
		unit->type = ENERGYUNIT;
		unit->unitType = L"ENERGYUNIT";
	}
	else if (equalStr(unitType, L".FORCEUNIT.")) {
		unit->type = FORCEUNIT;
		unit->unitType = L"FORCEUNIT";
	}
	else if (equalStr(unitType, L".FREQUENCYUNIT.")) {
		unit->type = FREQUENCYUNIT;
		unit->unitType = L"FREQUENCYUNIT";
	}
	else if (equalStr(unitType, L".ILLUMINANCEUNIT.")) {
		unit->type = ILLUMINANCEUNIT;
		unit->unitType = L"ILLUMINANCEUNIT";
	}
	else if (equalStr(unitType, L".INDUCTANCEUNIT.")) {
		unit->type = INDUCTANCEUNIT;
		unit->unitType = L"INDUCTANCEUNIT";
	}
	else if (equalStr(unitType, L".LENGTHUNIT.")) {
		unit->type = LENGTHUNIT;
		unit->unitType = L"LENGTHUNIT";
	}
	else if (equalStr(unitType, L".LUMINOUSFLUXUNIT.")) {
		unit->type = LUMINOUSFLUXUNIT;
		unit->unitType = L"LUMINOUSFLUXUNIT";
	}
	else if (equalStr(unitType, L".LUMINOUSINTENSITYUNIT.")) {
		unit->type = LUMINOUSINTENSITYUNIT;
		unit->unitType = L"LUMINOUSINTENSITYUNIT";
	}
	else if (equalStr(unitType, L".MAGNETICFLUXDENSITYUNIT.")) {
		unit->type = MAGNETICFLUXDENSITYUNIT;
		unit->unitType = L"MAGNETICFLUXDENSITYUNIT";
	}
	else if (equalStr(unitType, L".MAGNETICFLUXUNIT.")) {
		unit->type = MAGNETICFLUXUNIT;
		unit->unitType = L"MAGNETICFLUXUNIT";
	}
	else if (equalStr(unitType, L".MASSUNIT.")) {
		unit->type = MASSUNIT;
		unit->unitType = L"MASSUNIT";
	}
	else if (equalStr(unitType, L".PLANEANGLEUNIT.")) {
		unit->type = PLANEANGLEUNIT;
		unit->unitType = L"PLANEANGLEUNIT";
	}
	else if (equalStr(unitType, L".POWERUNIT.")) {
		unit->type = POWERUNIT;
		unit->unitType = L"POWERUNIT";
	}
	else if (equalStr(unitType, L".PRESSUREUNIT.")) {
		unit->type = PRESSUREUNIT;
		unit->unitType = L"PRESSUREUNIT";
	}
	else if (equalStr(unitType, L".RADIOACTIVITYUNIT.")) {
		unit->type = RADIOACTIVITYUNIT;
		unit->unitType = L"RADIOACTIVITYUNIT";
	}
	else if (equalStr(unitType, L".SOLIDANGLEUNIT.")) {
		unit->type = SOLIDANGLEUNIT;
		unit->unitType = L"SOLIDANGLEUNIT";
	}
	else if (equalStr(unitType, L".THERMODYNAMICTEMPERATUREUNIT.")) {
		unit->type = THERMODYNAMICTEMPERATUREUNIT;
		unit->unitType = L"THERMODYNAMICTEMPERATUREUNIT";
	}
	else if (equalStr(unitType, L".TIMEUNIT.")) {
		unit->type = TIMEUNIT;
		unit->unitType = L"TIMEUNIT";
	}
	else if (equalStr(unitType, L".VOLUMEUNIT.")) {
		unit->type = VOLUMEUNIT;
		unit->unitType = L"VOLUMEUNIT";
	}
	else if (equalStr(unitType, L".USERDEFINED.")) {
		unit->type = USERDEFINED;
		unit->unitType = L"USERDEFINED";
	}
	else {
		//ASSERT(false);
	}
}

void IFCConvector::UnitAddPrefix(IFC__SIUNIT * unit, wchar_t * prefix)
{
	//
	//	prefix
	//
	if (equalStr(prefix, L".EXA.")) {
		unit->prefix = L"Exa";
	}
	else if (equalStr(prefix, L".PETA.")) {
		unit->prefix = L"Peta";
	}
	else if (equalStr(prefix, L".TERA.")) {
		unit->prefix = L"Tera";
	}
	else if (equalStr(prefix, L".GIGA.")) {
		unit->prefix = L"Giga";
	}
	else if (equalStr(prefix, L".MEGA.")) {
		unit->prefix = L"Mega";
	}
	else if (equalStr(prefix, L".KILO.")) {
		unit->prefix = L"Kilo";
	}
	else if (equalStr(prefix, L".HECTO.")) {
		unit->prefix = L"Hecto";
	}
	else if (equalStr(prefix, L".DECA.")) {
		unit->prefix = L"Deca";
	}
	else if (equalStr(prefix, L".DECI.")) {
		unit->prefix = L"Deci";
	}
	else if (equalStr(prefix, L".CENTI.")) {
		unit->prefix = L"Centi";
	}
	else if (equalStr(prefix, L".MILLI.")) {
		unit->prefix = L"Milli";
	}
	else if (equalStr(prefix, L".MICRO.")) {
		unit->prefix = L"Micro";
	}
	else if (equalStr(prefix, L".NANO.")) {
		unit->prefix = L"Nano";
	}
	else if (equalStr(prefix, L".PICO.")) {
		unit->prefix = L"Pico";
	}
	else if (equalStr(prefix, L".FEMTO.")) {
		unit->prefix = L"Femto";
	}
	else if (equalStr(prefix, L".ATTO.")) {
		unit->prefix = L"Atto";
	}
	else {
		//ASSERT(prefix == 0);
	}
}

void IFCConvector::UnitAddName(IFC__SIUNIT * unit, wchar_t * name)
{
	//
	//	name
	//
	if (equalStr(name, L".AMPERE.")) {
		unit->name = L"Ampere";
	}
	else if (equalStr(name, L".BECQUEREL.")) {
		unit->name = L"Becquerel";
	}
	else if (equalStr(name, L".CANDELA.")) {
		unit->name = L"Candela";
	}
	else if (equalStr(name, L".COULOMB.")) {
		unit->name = L"Coulomb";
	}
	else if (equalStr(name, L".CUBIC_METRE.")) {
		unit->name = L"Cubic Metre";
	}
	else if (equalStr(name, L".DEGREE_CELSIUS.")) {
		unit->name = L"Degree Celcius";
	}
	else if (equalStr(name, L".FARAD.")) {
		unit->name = L"Farad";
	}
	else if (equalStr(name, L".GRAM.")) {
		unit->name = L"Gram";
	}
	else if (equalStr(name, L".GRAY.")) {
		unit->name = L"Gray";
	}
	else if (equalStr(name, L".HENRY.")) {
		unit->name = L"Henry";
	}
	else if (equalStr(name, L".HERTZ.")) {
		unit->name = L"Hertz";
	}
	else if (equalStr(name, L".JOULE.")) {
		unit->name = L"Joule";
	}
	else if (equalStr(name, L".KELVIN.")) {
		unit->name = L"Kelvin";
	}
	else if (equalStr(name, L".LUMEN.")) {
		unit->name = L"Lumen";
	}
	else if (equalStr(name, L".LUX.")) {
		unit->name = L"Lux";
	}
	else if (equalStr(name, L".METRE.")) {
		unit->name = L"Metre";
	}
	else if (equalStr(name, L".MOLE.")) {
		unit->name = L"Mole";
	}
	else if (equalStr(name, L".NEWTON.")) {
		unit->name = L"Newton";
	}
	else if (equalStr(name, L".OHM.")) {
		unit->name = L"Ohm";
	}
	else if (equalStr(name, L".PASCAL.")) {
		unit->name = L"Pascal";
	}
	else if (equalStr(name, L".RADIAN.")) {
		unit->name = L"Radian";
	}
	else if (equalStr(name, L".SECOND.")) {
		unit->name = L"Second";
	}
	else if (equalStr(name, L".SIEMENS.")) {
		unit->name = L"Siemens";
	}
	else if (equalStr(name, L".SIEVERT.")) {
		unit->name = L"Sievert";
	}
	else if (equalStr(name, L".SQUARE_METRE.")) {
		unit->name = L"Square Metre";
	}
	else if (equalStr(name, L".STERADIAN.")) {
		unit->name = L"Steradian";
	}
	else if (equalStr(name, L".TESLA.")) {
		unit->name = L"Tesla";
	}
	else if (equalStr(name, L".VOLT.")) {
		unit->name = L"Volt";
	}
	else if (equalStr(name, L".WATT.")) {
		unit->name = L"Watt";
	}
	else if (equalStr(name, L".WEBER.")) {
		unit->name = L"Weber";
	}
	else {
		//ASSERT(false);
	}
}

//------------------------------------------------------------------------------------

UGbool IFCConvector::ParseFile(std::wstring filename)
{
	// 加载IFC文件
	ClearUpIfcModel();

	m_firstItemWithGeometryPassed = false;
	setStringUnicode(1);
	m_ifcModel = sdaiOpenModelBNUnicode(0, (char*)filename.c_str(), 0);
	if (m_ifcModel)
	{
		wchar_t	* fileSchema = 0;
		GetSPFFHeaderItem(m_ifcModel, 9, 0, sdaiUNICODE, (char**)&fileSchema);

		if (fileSchema == 0 ||
			contains(fileSchema, L"IFC2x3") ||
			contains(fileSchema, L"IFC2X3") ||
			contains(fileSchema, L"IFC2x2") ||
			contains(fileSchema, L"IFC2X2") ||
			contains(fileSchema, L"IFC2x_") ||
			contains(fileSchema, L"IFC2X_") ||
			contains(fileSchema, L"IFC20")) {
				sdaiCloseModel(m_ifcModel);

				m_ifcModel = sdaiOpenModelBNUnicode(0, (char*)filename.c_str(), (char*)m_ifcSchemaName_IFC2x3);
		}
		else {
			if (contains(fileSchema, L"IFC4x") ||
				contains(fileSchema, L"IFC4X")) {
					sdaiCloseModel(m_ifcModel);

					m_ifcModel = sdaiOpenModelBNUnicode(0, (char*)filename.c_str(), (char*)m_ifcSchemaName_IFC4x1);
			}
			else {
				if (contains(fileSchema, L"IFC4") ||
					contains(fileSchema, L"IFC2x4") ||
					contains(fileSchema, L"IFC2X4")) {
						sdaiCloseModel(m_ifcModel);

						m_ifcModel = sdaiOpenModelBNUnicode(0, (char*)filename.c_str(), (char*)m_ifcSchemaName_IFC4);
				}
				else {
					sdaiCloseModel(m_ifcModel);
					return FALSE;
				}
			}
		}
		// --------------------------------------------------
		if (!m_ifcModel)
		{
			return FALSE; 
		}
		//---------------------------------------------------
		m_ifcSpace_TYPE               = sdaiGetEntity(m_ifcModel, (char*)L"IFCSPACE");
		m_ifcDistributionElement_TYPE = sdaiGetEntity(m_ifcModel, (char*)L"IFCDISTRIBUTIONELEMENT");
		m_ifcElectricalElement_TYPE	  = sdaiGetEntity(m_ifcModel, (char*)L"IFCELECTRICALELEMENT");
		m_ifcElementAssembly_TYPE     = sdaiGetEntity(m_ifcModel, (char*)L"IFCELEMENTASSEMBLY");
		m_ifcElementComponent_TYPE    = sdaiGetEntity(m_ifcModel, (char*)L"IFCELEMENTCOMPONENT");
		m_ifcEquipmentElement_TYPE    = sdaiGetEntity(m_ifcModel, (char*)L"IFCEQUIPMENTELEMENT");
		m_ifcFeatureElement_TYPE      = sdaiGetEntity(m_ifcModel, (char*)L"IFCFEATUREELEMENT");
		m_ifcFeatureElementSubtraction_TYPE = sdaiGetEntity(m_ifcModel, (char*)L"IFCFEATUREELEMENTSUBTRACTION");
		m_ifcFurnishingElement_TYPE   = sdaiGetEntity(m_ifcModel, (char*)L"IFCFURNISHINGELEMENT");
		m_ifcReinforcingElement_TYPE  = sdaiGetEntity(m_ifcModel, (char*)L"IFCREINFORCINGELEMENT");
		m_ifcTransportElement_TYPE    = sdaiGetEntity(m_ifcModel, (char*)L"IFCTRANSPORTELEMENT");
		m_ifcVirtualElement_TYPE      = sdaiGetEntity(m_ifcModel, (char*)L"IFCVIRTUALELEMENT");

		QProgressDialog dialog;
		dialog.setModal(true);
		//dialog.setLabelText(QString("Progressing using %1 thread(s)...").arg(QThread::idealThreadCount()));

		dialog.setWindowTitle(QString::fromLocal8Bit("载入IFC文件..."));
		dialog.setLabelText(QString::fromLocal8Bit("载入...."));
		dialog.setMaximum(100);
		dialog.setMinimum(0);
		dialog.setValue(0);
		bool	hide = false;
		int		segmentationParts = 36;

		dialog.show();
		m_firstFreeIfcObject = GetChildrenRecursively(
			m_ifcModel,
			sdaiGetEntity(m_ifcModel, (char*)L"IFCPRODUCT"),
			m_firstFreeIfcObject,
			segmentationParts,
			hide
			);
		for (int i=0; i<=25; i++)
		{
			dialog.setValue(i);
		}
		m_firstFreeIfcObject = QueryIfcObjects(m_ifcModel, m_firstFreeIfcObject, L"IFCRELSPACEBOUNDARY", segmentationParts, hide);

		if (m_ifcObjects)
		{
			dialog.setLabelText(QString::fromLocal8Bit("载入材质...."));
			InitMaterials(m_ifcModel);
			for (int i = 26; i <= 50; i++)
			{
				dialog.setValue(i);
			}

			// 获取Geometry
			dialog.setLabelText(QString::fromLocal8Bit("载入几何数据...."));
			GenerateGeometryNestedCall(m_ifcModel);
			for (int i = 51; i <= 75; i++)
			{
				dialog.setValue(i);
			}

			// 获取Units
			dialog.setLabelText(QString::fromLocal8Bit("载入单位信息...."));
			GetUnits(m_ifcModel);
			for (int i = 76; i <= 99; i++)
			{
				dialog.setValue(i);
			}

			dialog.setLabelText(QString::fromLocal8Bit("构建属性表...."));
			dialog.setMaximum(m_instanceIFCObject.size());
			dialog.setValue(0);
			// 构建属性表
			std::map<int_t, IFC__OBJECT*>::iterator it= m_instanceIFCObject.begin();
			int_t i = 0;
			while (it != m_instanceIFCObject.end())
			{
				if (dialog.wasCanceled())
				{
					break;
				}
				dialog.setValue(i);
				IFC__OBJECT* pIFCObj = it->second;
				if (pIFCObj && !pIFCObj->hide)
				{
					IFC__PROPERTY__SET* ifcPropSet = NULL;
					CreateIfcInstanceProperties(m_ifcModel, &ifcPropSet, it->first,m_units);
					m_ifcProperty[it->first] = ifcPropSet;

					//int_t entity = sdaiGetInstanceType(it->first);
					//int_t index = 0;
					//CreateObjProList(it->first,entity, index);
				}
				it++;
				i++;
			}
			dialog.hide();
			return TRUE;
		}
		else
		{
			dialog.hide();
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

UGbool IFCConvector::ExportUDB(UGString filename, UGString datasetName)
{
	datasetName.Replace(_U("-"), _U("_"));

	// 链接数据集
	OgdcDataSource* pUDBDataSource = OgdcProviderManager::CreateOgdcDataSource(OGDC::oeFile);
	pUDBDataSource->m_nEngineClass = 2;
	pUDBDataSource->m_connection.m_strServer = filename;//_U("E:/Data/workspace.udb");
	if (!((OgdcDataSource*)pUDBDataSource)->Open())
	{
		QMessageBox::warning(NULL, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("打开数据源失败！"));
		delete pUDBDataSource;
		pUDBDataSource = NULL;
		return FALSE;
	}

	// 创建模型数据集
	OgdcDatasetVectorInfo tempDatasetVectorInfo;
	tempDatasetVectorInfo.m_strName = datasetName;
	tempDatasetVectorInfo.m_nType = OgdcDataset::Model;

	OgdcDataset* pModelDataSet = pUDBDataSource->GetDataset(datasetName);
	if (pModelDataSet != NULL)
	{
		pUDBDataSource->DeleteDataset(datasetName);
		pModelDataSet = NULL;
	}

	OgdcDatasetVector* pModelDataVector = pUDBDataSource->CreateDatasetVector(tempDatasetVectorInfo);

	if (pModelDataVector == NULL)
	{
		QMessageBox::warning(NULL, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("创建数据集失败！"));
		delete pUDBDataSource;
		pUDBDataSource = NULL;
		return FALSE;
	}
	if (m_bIsPlaceSphere)
	{
		OgdcCoordSys prjCoordSys;// 默认为经纬度坐标
		pModelDataVector->SetCoordSys(prjCoordSys);
	}

	// 创建属性字段
	UGFieldInfos fieldInfos;
	std::set<UGString>::iterator it;// = m_objPropList.begin();
// 	while (it != m_objPropList.end())
// 	{
// 		UGFieldInfo fieldInfo;
// 		fieldInfo.m_strName = *it;
// 		fieldInfo.m_nType = UGFieldInfo::Text;
// 		fieldInfo.m_nSize = 256;
// 		fieldInfos.Add(fieldInfo);
// 
// 		it++;
// 	}

	it = m_propList.begin();
	UGuint i = 1;
	while (it != m_propList.end())
	{	
		UGString  strPropName =*it;
		UGuint nLength = strPropName.GetLength();

		UGFieldInfo fieldInfo;
		fieldInfo.m_nType = UGFieldInfo::Text;
		fieldInfo.m_nSize = 256;

		if (nLength > 20)
		{
			fieldInfo.m_strName = _U("Property_") + UGString::From(i);
			fieldInfo.m_strForeignName = *it;

			m_propForeign[fieldInfo.m_strForeignName] = fieldInfo.m_strName;
		}
		else
		{
			fieldInfo.m_strName = *it;
		}
		fieldInfos.Add(fieldInfo);

		if (nLength > 20)
		{
			fieldInfo.m_strName = _U("Property_") + UGString::From(i) + _U("_Unit");
			fieldInfo.m_strForeignName = *it + _U("_Unit");
		}
		else
		{
			fieldInfo.m_strName = *it + _U("_Unit");
		}
		fieldInfos.Add(fieldInfo);

		i++;
		it++;
	}

	pModelDataVector->CreateFields(fieldInfos);

	OgdcQueryDef def;
	def.m_nCursorType = OgdcQueryDef::OpenDynamic;
	def.m_nOptions = OgdcQueryDef::Both;
	OgdcRecordset* pModelRecordSet = pModelDataVector->Query(def);

	IFCObjToGeoModel(pModelRecordSet);

	if (pModelRecordSet)
	{
		pModelDataVector->ReleaseRecordset(pModelRecordSet);
		pModelRecordSet = NULL;
	}

	if (pUDBDataSource)
	{
		delete pUDBDataSource;
		pUDBDataSource = NULL;
		pModelDataVector = NULL;
	}

	return TRUE;
}

void IFCConvector::setPosition(UGPoint3D & point)
{
	m_Pos = point;
}

void IFCConvector::setIsPlaceSphere(UGbool bPlaceSphere)
{
	m_bIsPlaceSphere = bPlaceSphere;
}

//-----------------------------------------------------------------------------------------------
void IFCConvector::DeleteIfcObject(IFC__OBJECT* ifcObject)
{
	if (ifcObject->vertices) {
		delete[] ifcObject->vertices;
	}
	if (ifcObject->indicesForPoints) {
		delete[] ifcObject->indicesForPoints;
	}
	if (ifcObject->indicesForLines) {
		delete[] ifcObject->indicesForLines;
	}
	if (ifcObject->indicesForFaces) {
		delete[] ifcObject->indicesForFaces;
	}
	if (ifcObject->indicesForLinesWireFrame) {
		delete[] ifcObject->indicesForLinesWireFrame;
	}

	// 	STRUCT_MATERIALS	* materials = ifcObject->materials;
	// 	while (materials) {
	// 		STRUCT_MATERIALS	* materialToRemove = materials;
	// 		materials = materials->next;
	// 
	// 		delete  materialToRemove;
	// 	}

	delete	ifcObject;
}

void IFCConvector::ClearUpIfcModel()
{
	if (m_ifcObjects)
	{
		IFC__OBJECT	* ifcObject = m_ifcObjects;
		while (ifcObject) {
			IFC__OBJECT	* ifcObjectToRemove = ifcObject;
			ifcObject = ifcObject->next;

			DeleteIfcObject(ifcObjectToRemove);
		}
		m_ifcObjects = NULL;
		m_firstFreeIfcObject = &m_ifcObjects;
	}

	m_instanceIFCObject.clear();

	ClearUnits();

	ClearMaterials();

	ClearPropertys();
}

void IFCConvector::IFCObjToGeoModel(OgdcRecordset* pRecordset)
{
	UGuint nCount = m_instanceIFCObject.size();
	if (nCount == 0)
	{
		return;
	}

	IFC__SIUNIT* pUnits = m_units;
	// 获取单位
	wchar_t* strUnit = GetUnit(m_units, L"LENGTHUNIT");
	double unit = 1.0;
	if (equalStr(strUnit, L"Milli Metre"))
	{
		unit = 1000.0;
	}
	else if(equalStr(strUnit,L"Centi Metre"))
	{
		unit = 100.0;
	}
	else if (equalStr(strUnit, L"Deci Metre"))
	{
		unit = 10.0;
	}

	QProgressDialog dialog;
	dialog.setModal(true);

	dialog.setWindowTitle(QString::fromLocal8Bit("导出数据集..."));
	dialog.setLabelText(QString::fromLocal8Bit("导出......"));
	dialog.setMaximum(m_instanceIFCObject.size()-1);
	dialog.setMinimum(0);
	dialog.show();

	std::map<int_t, IFC__OBJECT*>::iterator it = m_instanceIFCObject.begin();
	UGuint i = 0;
	while (it != m_instanceIFCObject.end())
	{
		if (dialog.wasCanceled())
		{
			break;
		}
		dialog.setValue(i);
		IFC__OBJECT* pIFCObj = it->second;

		if (pIFCObj)
		{
			if (pIFCObj->hide)
			{
				it++;
				i++;
				continue;
			}
			if (pIFCObj->noPrimitivesForFaces > 0)
			{
				// 顶点
				int_t nVerCount = pIFCObj->noVertices;
				float* pVertices = pIFCObj->vertices;

				// 索引
				UGuint nIndicesCount = pIFCObj->noPrimitivesForFaces * 3;
				int32_t* pIndices = pIFCObj->indicesForFaces;
				std::vector<UGuint> ids;

				// 去除冗余点
				UGReindexer3d reindexer;
				for (UGuint j = 0; j < nIndicesCount; j++)
				{
					UGuint index = pIndices[j];

					float x = pVertices[6 * index];
					float y = pVertices[6 * index + 1];
					float z = pVertices[6 * index + 2];

					const UGVector3d v(x , y , z);
					UGuint id = reindexer.Lookup(v);
					ids.push_back(id);
				}

				// 新的顶点和索引
				const UGVector3d* pVert = reindexer.GetArray();
				nVerCount = reindexer.GetSize();
				nIndicesCount = ids.size();

				// 骨架顶点包
				UGVertexDataPackageExact* pVertexPackExat = new UGVertexDataPackageExact;
				UGuint nVertexDim = pVertexPackExat->m_nVertexDimension;
				pVertexPackExat->SetVertexNum(nVerCount);
				//pVertexPackExat->SetNormalNum(nVerCount);
				//pVertexPackExat->SetColorNum(nVerCount);
				for (int_t i = 0; i < nVerCount; i++)
				{
					// 顶点
					pVertexPackExat->m_pVertices[nVertexDim*i + 0] = pVert[i].x / unit;
					pVertexPackExat->m_pVertices[nVertexDim*i + 1] = pVert[i].y / unit;
					pVertexPackExat->m_pVertices[nVertexDim*i + 2] = pVert[i].z / unit;
					// 法线
					//pVertexPackExat->m_pNormals[nVertexDim * i + 0] = (UGdouble)pVertices[6 * i + 3];
					//pVertexPackExat->m_pNormals[nVertexDim * i + 1] = (UGdouble)pVertices[6 * i + 4];
					//pVertexPackExat->m_pNormals[nVertexDim * i + 2] = (UGdouble)pVertices[6 * i + 5];
					// 顶点颜色
					//UGColorValue3D value;
					//value.SetValue(255, 0, 0, 255);	
					//pVertexPackExat->m_pVertexColor[i] = value.GetAsLongARGB();
				}
				//----------------------------
				// 骨架索引
				UGIndexPackage* pIndexPackage = new UGIndexPackage;
				//pIndexPackage->m_strPassName.Add(UG_MATERTIALNAME_DEFAULT);
				if (nIndicesCount > 65536)
				{
					pIndexPackage->m_enIndexType = IT_32BIT;
				}
				else
				{
					pIndexPackage->m_enIndexType = IT_16BIT;
				}


				pIndexPackage->SetIndexNum(nIndicesCount);
				for (UGuint i=0; i<nIndicesCount; i++)
				{
					if (pIndexPackage->m_enIndexType == IT_16BIT)
					{
						pIndexPackage->m_pIndexes[i] = ids[i];
					}
					else
					{
						UGuint* pUintIndex = (UGuint*)pIndexPackage->m_pIndexes;
						pUintIndex[i] = ids[i];
					}
				}

				UGString strName, strMateriaName;
				strName.Format(_U("%d"), it->first);
				strMateriaName.Format(_U("%d_Material"), it->first);

				UGModelMaterial* pModelMaterial = IFCMaterialToUGMaterial(strMateriaName, pIFCObj->materials);
				pIndexPackage->m_strPassName.Add(pModelMaterial->m_strName);

				UGModelSkeleton* pModelSkeleton = new UGModelSkeleton;

				pModelSkeleton->m_strName = strName;
				pModelSkeleton->m_strMaterialName = pModelMaterial->m_strName;
				pModelSkeleton->SetExactDataTag(true);
				pModelSkeleton->SetExactDataPackRef(pVertexPackExat, pIndexPackage);

				UGModelEMapPack mapPack;
				mapPack.AddSkeleton(pModelSkeleton);
				mapPack.AddMaterial(pModelMaterial);

				UGModelGeode* pGeode = new UGModelGeode();
				pGeode->MakeFrom(mapPack);

				UGModelNode* pModelNode = new UGModelNode;
				pModelNode->AddGeode(pGeode , pModelNode->GetDataPatcheCount() - 1);

				UGGeoModelPro* pModelPro = new UGGeoModelPro;
				pModelPro->SetModelNode(pModelNode);
				pModelPro->SetSpherePlaced(m_bIsPlaceSphere);
				pModelPro->SetPosition(m_Pos);

				pRecordset->AddNew(pModelPro);

 				int_t entity = sdaiGetInstanceType(it->first);
 				int_t index = 0;
 				//CreateObjProList(it->first,entity, index,pRecordset);
 				IFCPropToFiled(pRecordset, m_ifcProperty[it->first]);
				pRecordset->Update();

				delete pModelNode;
				pModelNode = NULL;
				delete pModelPro;
			}
		}

		it++;
		i++;
	}
	dialog.hide();
}

IFC__OBJECT** IFCConvector::GetChildrenRecursively(int_t ifcModel,int_t ifcParentEntity,IFC__OBJECT** firstFreeIfcObject, int_t segmentationParts,bool hide)
{
	int_t	* ifcEntityExtend = sdaiGetEntityExtent(ifcModel, ifcParentEntity),
		cnt = sdaiGetMemberCount(ifcEntityExtend);

	if ((ifcParentEntity == m_ifcSpace_TYPE) ||
		(ifcParentEntity == m_ifcFeatureElementSubtraction_TYPE)) {
			hide = true;
	}

	if ((ifcParentEntity == m_ifcDistributionElement_TYPE) ||
		(ifcParentEntity == m_ifcElectricalElement_TYPE) ||
		(ifcParentEntity == m_ifcElementAssembly_TYPE) ||
		(ifcParentEntity == m_ifcElementComponent_TYPE) ||
		(ifcParentEntity == m_ifcEquipmentElement_TYPE) ||
		(ifcParentEntity == m_ifcFeatureElement_TYPE) ||
		(ifcParentEntity == m_ifcFurnishingElement_TYPE) ||
		(ifcParentEntity == m_ifcTransportElement_TYPE) ||
		(ifcParentEntity == m_ifcVirtualElement_TYPE)) {
			segmentationParts = 12;
	}

	if (ifcParentEntity == m_ifcReinforcingElement_TYPE) {
		segmentationParts = 6;// 12;
	}

	if (cnt) {
		wchar_t	* ifcParentEntityName = nullptr;
		engiGetEntityName(ifcParentEntity, sdaiUNICODE, (char**)&ifcParentEntityName);

		firstFreeIfcObject = QueryIfcObjects(ifcModel, firstFreeIfcObject, ifcParentEntityName ,segmentationParts,hide);
	}

	cnt = engiGetEntityCount(ifcModel);
	for (int_t i = 0; i < cnt; i++) {
		int_t ifcEntity = engiGetEntityElement(ifcModel, i);
		if (engiGetEntityParent(ifcEntity) == ifcParentEntity) {
			firstFreeIfcObject = GetChildrenRecursively(ifcModel, ifcEntity, firstFreeIfcObject, segmentationParts,hide);
		}
	}

	return	firstFreeIfcObject;
}

IFC__OBJECT** IFCConvector::QueryIfcObjects(int_t ifcModel,IFC__OBJECT** firstFreeIfcObject, wchar_t* entityName,int_t segmentationParts,bool hide)
{
	int_t	i, *ifcObjectInstances, noIfcObjectInstances;

	ifcObjectInstances = sdaiGetEntityExtentBN(ifcModel, (char*)entityName);

	noIfcObjectInstances = sdaiGetMemberCount(ifcObjectInstances);
	if (noIfcObjectInstances) {
		int_t	ifcEntity = sdaiGetEntity(ifcModel, (char*)entityName);
		for (i = 0; i < noIfcObjectInstances; ++i) {
			int_t	ifcObjectInstance = 0;
			engiGetAggrElement(ifcObjectInstances, i, sdaiINSTANCE, &ifcObjectInstance);

			IFC__OBJECT	* ifcObject = CreateIfcObject(ifcEntity, ifcObjectInstance, entityName, segmentationParts,hide);
			(*firstFreeIfcObject) = ifcObject;
			firstFreeIfcObject = &ifcObject->next;

			//allIFCObject[ifcObjectInstance] = ifcObject;
		}
	}

	return	firstFreeIfcObject;
}

IFC__OBJECT* IFCConvector::CreateIfcObject(int_t ifcEntity,int_t ifcInstance, wchar_t* entityName, int_t segmentationParts,bool hide)
{
	IFC__OBJECT	* ifcObject = new IFC__OBJECT;

	// 	if (hide) {
	// 		ifcObject->selectState = TI_UNCHECKED;
	// 	}
	// 	else {
	// 		ifcObject->selectState = TI_CHECKED;
	// 	}

	ifcObject->noVertices = 0;
	ifcObject->vertices = 0;

	ifcObject->next = nullptr;

	ifcObject->entityName = entityName;
	ifcObject->hide = hide;
	ifcObject->segmentationParts = segmentationParts;

	ifcObject->materials = 0;

	//ifcObject->vecMin.x = 0;
	//ifcObject->vecMin.y = 0;
	//ifcObject->vecMin.z = 0;
	//ifcObject->vecMax.x = 0;
	//ifcObject->vecMax.y = 0;
	//ifcObject->vecMax.z = 0;

	ifcObject->ifcInstance = ifcInstance;
	ifcObject->ifcEntity = ifcEntity;
	//	if	(hide) {
	//		ifcObject->ifcItemCheckedAtStartup = false;
	//	} else {
	//		ifcObject->ifcItemCheckedAtStartup = true;
	//	}
	//	ifcObject->treeItemGeometry = 0;
	//	ifcObject->treeItemProperties = 0;

	ifcObject->noVertices = 0;
	ifcObject->vertices = 0;

	ifcObject->vertexOffset = 0;

	ifcObject->noPrimitivesForPoints = 0;
	ifcObject->indicesForPoints = 0;
	ifcObject->indexOffsetForPoints = 0;

	ifcObject->noPrimitivesForLines = 0;
	ifcObject->indicesForLines = 0;
	ifcObject->indexOffsetForLines = 0;

	ifcObject->noPrimitivesForFaces = 0;
	ifcObject->indicesForFaces = 0;
	ifcObject->indexOffsetForFaces = 0;

	ifcObject->noPrimitivesForWireFrame = 0;
	ifcObject->indicesForLinesWireFrame = 0;
	ifcObject->indexOffsetForWireFrame = 0;

	return	ifcObject;
}

void IFCConvector::GenerateGeometryNestedCall(int_t ifcModel)
{
	//int	objectCnt = 0;
	IFC__OBJECT	* ifcObject = m_ifcObjects;
	// 	while (ifcObject) {
	// 		objectCnt++;
	// 		ifcObject = ifcObject->next;
	// 	}
	// 
	// 	ifcObject = *
	while (ifcObject) {
		//
		//	Get Geometry
		//
		int_t	setting = 0, mask = 0;
		mask += flagbit2;        //    PRECISION (32/64 bit)
		mask += flagbit3;        //	   INDEX ARRAY (32/64 bit)
		mask += flagbit5;        //    NORMALS
		mask += flagbit8;        //    TRIANGLES
		mask += flagbit9;        //    LINES
		mask += flagbit10;       //    POINTS
		mask += flagbit12;       //    WIREFRAME

		setting += 0;		     //    SINGLE PRECISION (float)
		setting += 0;            //    32 BIT INDEX ARRAY (Int32)
		setting += flagbit5;     //    NORMALS ON
		setting += flagbit8;     //    TRIANGLES ON
		setting += flagbit9;     //    LINES ON
		setting += flagbit10;    //    POINTS ON
		setting += flagbit12;    //    WIREFRAME ON
		setFormat(ifcModel, setting, mask);

		setFilter(ifcModel, flagbit1, flagbit1);

		/**/
		circleSegments(ifcObject->segmentationParts, 5);

		GenerateWireFrameGeometry(ifcModel, ifcObject);
		cleanMemory(ifcModel, 0);

		ifcObject = ifcObject->next;
	}
}

void IFCConvector::GenerateWireFrameGeometry(int_t ifcModel, IFC__OBJECT* ifcObject)
{
	if (ifcObject  &&  ifcObject->ifcInstance)
	{
		int64_t noVertices = 0, noIndices = 0;
		CalculateInstance((int_t)ifcObject->ifcInstance, &noVertices, &noIndices, nullptr);

		int64_t	owlModel = 0,
			owlInstance = 0;

		if (noVertices  &&  noIndices) {
			owlGetModel(
				ifcModel,
				&owlModel
				);

			owlGetInstance(
				ifcModel,
				ifcObject->ifcInstance,
				&owlInstance
				);
		}

		if (noVertices  &&  noIndices) {
			ifcObject->noVertices = (int_t)noVertices;
			ifcObject->vertices = new float[(int)noVertices * 6];
			int32_t	* indices = new int32_t[(int)noIndices];

			UpdateInstanceVertexBuffer(owlInstance, ifcObject->vertices);
			UpdateInstanceIndexBuffer(owlInstance, indices);

			//			int64_t	owlModel = 0, owlInstance = 0;
			if (m_firstItemWithGeometryPassed == false) {
				//				owlGetModel(ifcModel, &owlModel);
				//				owlGetInstance(ifcModel, ifcObject->ifcInstance, &owlInstance);
				//				ASSERT(owlModel  &&  owlInstance);

				double	transformationMatrix[12], minVector[3], maxVector[3];
				SetBoundingBoxReference(owlInstance, transformationMatrix, minVector, maxVector);

				if ((-1000000 > transformationMatrix[9] || transformationMatrix[9] > 1000000) ||
					(-1000000 > transformationMatrix[10] || transformationMatrix[10] > 1000000) ||
					(-1000000 > transformationMatrix[11] || transformationMatrix[11] > 1000000)) {
						SetVertexBufferOffset(ifcModel, -transformationMatrix[9], -transformationMatrix[10], -transformationMatrix[11]);

						ClearedInstanceExternalBuffers(owlInstance);
						UpdateInstanceVertexBuffer((__int64)ifcObject->ifcInstance, ifcObject->vertices);
						UpdateInstanceIndexBuffer((__int64)ifcObject->ifcInstance, indices);
				}
			}

			m_firstItemWithGeometryPassed = true;

			ifcObject->noPrimitivesForWireFrame = 0;
			//ASSERT(ifcObject->indicesForLinesWireFrame == 0);
			int32_t	* indicesForLinesWireFrame = new int32_t[2 * (int)noIndices];

			ifcObject->noVertices = (int_t)noVertices;
			//ASSERT(ifcObject->indicesForFaces == 0);
			int32_t	* indicesForFaces = new int32_t[(int)noIndices];
			int32_t	* indicesForLines = new int32_t[(int)noIndices];
			int32_t	* indicesForPoints = new int32_t[(int)noIndices];

			int_t	faceCnt = getConceptualFaceCnt(ifcObject->ifcInstance);
			int_t	* maxIndex = new int_t[faceCnt],
				*primitivesForFaces = new int_t[faceCnt];

			for (int_t j = 0; j < faceCnt; j++) {
				int_t	startIndexTriangles = 0, noIndicesTrangles = 0,
					startIndexLines = 0, noIndicesLines = 0,
					startIndexPoints = 0, noIndicesPoints = 0,
					startIndexFacesPolygons = 0, noIndicesFacesPolygons = 0;

				getConceptualFaceEx(
					ifcObject->ifcInstance, j,
					&startIndexTriangles, &noIndicesTrangles,
					&startIndexLines, &noIndicesLines,
					&startIndexPoints, &noIndicesPoints,
					&startIndexFacesPolygons, &noIndicesFacesPolygons,
					0, 0
					);

				if (j) {
					maxIndex[j] = maxIndex[j - 1];
				}
				else {
					maxIndex[j] = 0;
				}

				if (noIndicesTrangles  &&  maxIndex[j] < startIndexTriangles + noIndicesTrangles) { maxIndex[j] = startIndexTriangles + noIndicesTrangles; }
				if (noIndicesLines  &&  maxIndex[j] < startIndexLines + noIndicesLines) { maxIndex[j] = startIndexLines + noIndicesLines; }
				if (noIndicesPoints  &&  maxIndex[j] < startIndexPoints + noIndicesPoints) { maxIndex[j] = startIndexPoints + noIndicesPoints; }
				if (noIndicesFacesPolygons  &&  maxIndex[j] < startIndexFacesPolygons + noIndicesFacesPolygons) { maxIndex[j] = startIndexFacesPolygons + noIndicesFacesPolygons; }

				int_t	i = 0;
				while (i < noIndicesTrangles) {
					indicesForFaces[ifcObject->noPrimitivesForFaces * 3 + i] = indices[startIndexTriangles + i];
					i++;
				}
				ifcObject->noPrimitivesForFaces += noIndicesTrangles / 3;
				primitivesForFaces[j] = noIndicesTrangles / 3;

				i = 0;
				while (i < noIndicesLines) {
					indicesForLines[ifcObject->noPrimitivesForLines * 2 + i] = indices[startIndexLines + i];
					i++;
				}
				ifcObject->noPrimitivesForLines += noIndicesLines / 2;

				i = 0;
				while (i < noIndicesPoints) {
					indicesForPoints[ifcObject->noPrimitivesForPoints * 1 + i] = indices[startIndexPoints + i];
					i++;
				}
				ifcObject->noPrimitivesForPoints += noIndicesPoints / 1;

				i = 0;
				int32_t	lastItem = -1;
				while (i < noIndicesFacesPolygons) {
					if (lastItem >= 0 && indices[startIndexFacesPolygons + i] >= 0) {
						indicesForLinesWireFrame[2 * ifcObject->noPrimitivesForWireFrame + 0] = lastItem;
						indicesForLinesWireFrame[2 * ifcObject->noPrimitivesForWireFrame + 1] = indices[startIndexFacesPolygons + i];
						ifcObject->noPrimitivesForWireFrame++;
					}
					lastItem = indices[startIndexFacesPolygons + i];
					i++;
				}
			}

			ifcObject->indicesForPoints = new int32_t[3 * ifcObject->noPrimitivesForPoints];
			ifcObject->indicesForLines = new int32_t[3 * ifcObject->noPrimitivesForLines];
			ifcObject->indicesForFaces = new int32_t[3 * ifcObject->noPrimitivesForFaces];
			ifcObject->indicesForLinesWireFrame = new int32_t[2 * ifcObject->noPrimitivesForWireFrame];

			memcpy(ifcObject->indicesForPoints, indicesForPoints, 1 * ifcObject->noPrimitivesForPoints * sizeof(int32_t));
			memcpy(ifcObject->indicesForLines, indicesForLines, 2 * ifcObject->noPrimitivesForLines * sizeof(int32_t));
			memcpy(ifcObject->indicesForFaces, indicesForFaces, 3 * ifcObject->noPrimitivesForFaces * sizeof(int32_t));
			memcpy(ifcObject->indicesForLinesWireFrame, indicesForLinesWireFrame, 2 * ifcObject->noPrimitivesForWireFrame * sizeof(int32_t));

			delete[]  indicesForLinesWireFrame;
			delete[]  indicesForFaces;
			delete[]  indicesForLines;
			delete[]  indicesForPoints;
			delete[]  indices;

			ifcObject->materials = IFCObjectMaterial(ifcModel, ifcObject->ifcInstance);

			if (ifcObject->materials) {
				IFC_MATERIALS	* materials = ifcObject->materials;
				if (materials->next) {
					__int64	indexBufferSize = 0, indexArrayOffset = 0, j = 0;
					while (materials) {
						//ASSERT(materials->__indexBufferSize >= 0);
						//ASSERT(materials->__noPrimitivesForFaces == 0);
						indexBufferSize += materials->__indexBufferSize;
						materials->__indexArrayOffset = indexArrayOffset;
						while (j < faceCnt  &&  maxIndex[j] <= indexBufferSize) {
							materials->__noPrimitivesForFaces += primitivesForFaces[j];
							indexArrayOffset += 3 * primitivesForFaces[j];
							j++;
						}
						materials = materials->next;
					}
					//ASSERT(j == faceCnt  &&  indexBufferSize == noIndices);
				}
				else {
					//ASSERT(materials->__indexBufferSize == -1);
					materials->__indexArrayOffset = 0;
					materials->__noPrimitivesForFaces = ifcObject->noPrimitivesForFaces;
				}
			}
			else {
				//ASSERT(false);
			}

			delete[]  primitivesForFaces;
			delete[]  maxIndex;

			m_instanceIFCObject[ifcObject->ifcInstance] = ifcObject;
		}
		else {
			//ASSERT(ifcObject->noVertices == 0 && ifcObject->noPrimitivesForPoints == 0 && ifcObject->noPrimitivesForLines == 0 && ifcObject->noPrimitivesForFaces == 0 && ifcObject->noPrimitivesForWireFrame == 0);

			//ifcObject->selectState = TI_NONE;
			//ASSERT(ifcObject->treeItemModel == nullptr);
			//ASSERT(ifcObject->treeItemSpaceBoundary == nullptr);
		}
	}
}
//---------------------------------------------------------------------------------------

IFC_MATERIALS* IFCConvector::IFCObjectMaterial(int_t ifcModel, int_t ifcInstance)
{
	IFC_MATERIAL_META_INFO	* materialMetaInfo = nullptr, ** ppMaterialMetaInfo = &materialMetaInfo;
	IFC_MATERIAL			* returnedMaterial = nullptr;

	int_t	ifcProductRepresentationInstance = 0;
	sdaiGetAttrBN(ifcInstance, (char*)L"Representation", sdaiINSTANCE, &ifcProductRepresentationInstance);
	if (ifcProductRepresentationInstance != 0) {
		GetRGB_productDefinitionShape(ifcModel, ifcProductRepresentationInstance, ppMaterialMetaInfo);
	}

	bool	noMaterialFound = false;
	if (materialMetaInfo && (*ppMaterialMetaInfo)->next == 0 && (*ppMaterialMetaInfo)->child == 0) {
		//ASSERT((*ppMaterialMetaInfo) == materialMetaInfo);
		if ((materialMetaInfo->material->ambient.A == -1) &&
			(materialMetaInfo->material->diffuse.A == -1) &&
			(materialMetaInfo->material->emissive.A == -1) &&
			(materialMetaInfo->material->specular.A == -1)) {
			noMaterialFound = true;
		}
	}

	if (noMaterialFound) {
		//
		//	Look for material associations
		//
		int_t	ifcRelAssociatesMaterialEntity = sdaiGetEntity(ifcModel, (char*)L"IFCRELASSOCIATESMATERIAL"),
			*ifcRelAssociatesAggr = nullptr,
			ifcRelAssociatesAggrCnt,
			i = 0;
		sdaiGetAttrBN(ifcInstance, (char*)L"HasAssociations", sdaiAGGR, &ifcRelAssociatesAggr);
		ifcRelAssociatesAggrCnt = sdaiGetMemberCount(ifcRelAssociatesAggr);
		while (i < ifcRelAssociatesAggrCnt) {
			int_t	ifcRelAssociatesInstance = 0;
			engiGetAggrElement(ifcRelAssociatesAggr, i, sdaiINSTANCE, &ifcRelAssociatesInstance);
			if (sdaiGetInstanceType(ifcRelAssociatesInstance) == ifcRelAssociatesMaterialEntity) {
				GetRGB_relAssociatesMaterial(ifcModel, ifcRelAssociatesInstance, materialMetaInfo->material);
			}
			i++;
		}
	}

	if (materialMetaInfo) {
		bool	unique = true, defaultColorIsUsed = false;
		IFC_MATERIAL	* material = nullptr;
		//DEBUG__localObjectColor = materialMetaInfo;
		MinimizeMaterialItems(materialMetaInfo, &material, &unique, &defaultColorIsUsed);

		if (unique) {
			returnedMaterial = material;
		}
		else {
			returnedMaterial = 0;
		}

		if (defaultColorIsUsed) {
			//
			//	Color not found, check if we can find colors via propertySets
			//
			/*			STRUCT__PROPERTY__SET	* propertySet = ifcItem->propertySets;
			while  (propertySet) {
			if	(equals(propertySet->name, L"Pset_Draughting")) {
			STRUCT__PROPERTY	* _property = propertySet->properties;
			while  (_property) {
			if	(equals(_property->name, L"Red")) {
			int_t	value = _wtoi(_property->nominalValue);
			}
			if	(equals(_property->name, L"Green")) {
			int_t	value = _wtoi(_property->nominalValue);
			}
			if	(equals(_property->name, L"Blue")) {
			int_t	value = _wtoi(_property->nominalValue);
			}
			_property = _property->next;
			}
			}
			propertySet = propertySet->next;
			}	//	*/
		}
	}

	if (returnedMaterial) {
		DeleteMaterialMetaInfo(materialMetaInfo);

		return	CreateMaterials(returnedMaterial, ifcModel, ifcInstance);
	}
	else {
		IFC_MATERIALS	* materials = 0, ** materialsRef = &materials;
		if (materialMetaInfo) {
			WalkThroughGeometry__transformation((int64_t)ifcModel, (int64_t)ifcInstance, &materialsRef, &materialMetaInfo);
		}

		DeleteMaterialMetaInfo(materialMetaInfo);

		if (materials) {
			return	materials;
		}
		else {
			return	CreateMaterials(m_firstMaterial, ifcModel, ifcInstance);
		}
	}
}

UGModelMaterial* IFCConvector::IFCMaterialToUGMaterial(UGString strMaterilName,IFC_MATERIALS * pIFCMaterials)
{
	UGModelMaterial* pModelMaterial = new UGModelMaterial;
	if (pIFCMaterials == NULL || pIFCMaterials->material == NULL)
	{
		pModelMaterial->MakeDefault();
		return pModelMaterial;
	}

	pModelMaterial->m_strName = strMaterilName;
	IFC_MATERIAL* pIFCMaterial = pIFCMaterials->material;
	IFC_MATERIAL* temp = pIFCMaterial->next;
	// 	while (temp)
	// 	{
	// 		pIFCMaterial = temp;
	// 		temp = pIFCMaterial->next;
	// 	}

	UGTechnique *t = new UGTechnique();
	pModelMaterial->mTechniques.push_back(t);

	UGPass* pass = t->createPass();
	pass->m_strName = strMaterilName;

	if (pIFCMaterial->ambient.R != -1)
	{
		pass->m_Ambient.r = pIFCMaterial->ambient.R;
		pass->m_Ambient.g = pIFCMaterial->ambient.G;
		pass->m_Ambient.b = pIFCMaterial->ambient.B;
		pass->m_Ambient.a = pIFCMaterial->transparency;
	}

	if (pIFCMaterial->diffuse.R != -1)
	{
		pass->m_Diffuse.r = pIFCMaterial->diffuse.R;
		pass->m_Diffuse.g = pIFCMaterial->diffuse.G;
		pass->m_Diffuse.b = pIFCMaterial->diffuse.B;
		pass->m_Diffuse.a = pIFCMaterial->transparency;
	}

	if (pIFCMaterial->specular.R != -1)
	{
		pass->m_Specular.r = pIFCMaterial->specular.R;
		pass->m_Specular.g = pIFCMaterial->specular.G;
		pass->m_Specular.b = pIFCMaterial->specular.B;
		pass->m_Specular.a = pIFCMaterial->transparency;
	}

	if (pIFCMaterial->emissive.R != -1)
	{
		pass->m_SelfIllumination.r = pIFCMaterial->emissive.R;
		pass->m_SelfIllumination.g = pIFCMaterial->emissive.G;
		pass->m_SelfIllumination.b = pIFCMaterial->emissive.B;
		pass->m_SelfIllumination.a = pIFCMaterial->transparency;
	}

	//pass->m_Specular.r = pIFCMaterial->specular.R;
	//pass->m_Specular.g = pIFCMaterial->specular.G;
	//pass->m_Specular.b = pIFCMaterial->specular.B;
	//pass->m_Specular.a = pIFCMaterial->transparency;

	//pass->m_SelfIllumination.r = pIFCMaterial->emissive.R;
	//pass->m_SelfIllumination.g = pIFCMaterial->emissive.G;
	//pass->m_SelfIllumination.b = pIFCMaterial->emissive.B;
	//pass->m_SelfIllumination.a = pIFCMaterial->transparency;

	pass->m_Shininess = pIFCMaterial->shininess;

	return pModelMaterial;
}

void IFCConvector::InitMaterials(int_t ifcObject)
{
	m_firstMaterial = NULL;
	m_lastMaterial  = NULL;

	m_defaultMaterial = CreateMaterial();

	m_defaultMaterial->ambient.R = -1;
	m_defaultMaterial->ambient.G = -1;
	m_defaultMaterial->ambient.B = -1;
	m_defaultMaterial->ambient.A = -1;
	m_defaultMaterial->diffuse.R = -1;
	m_defaultMaterial->diffuse.G = -1;
	m_defaultMaterial->diffuse.B = -1;
	m_defaultMaterial->diffuse.A = -1;
	m_defaultMaterial->specular.R = -1;
	m_defaultMaterial->specular.G = -1;
	m_defaultMaterial->specular.B = -1;
	m_defaultMaterial->specular.A = -1;
	m_defaultMaterial->emissive.R = -1;
	m_defaultMaterial->emissive.G = -1;
	m_defaultMaterial->emissive.B = -1;
	m_defaultMaterial->emissive.A = -1;

	m_defaultMaterial->transparency = 1;
	m_defaultMaterial->shininess = 1;

	m_defaultMaterial->active = true;

	m_rdfClassTransformation = GetClassByName((int64_t)ifcObject, "Transformation");
	m_rdfClassCollection = GetClassByName((int64_t)ifcObject, "Collection");

	m_owlDataTypePropertyExpressID = GetPropertyByName((int64_t)ifcObject, "expressID");

	m_owlObjectTypePropertyMatrix = GetPropertyByName((int64_t)ifcObject, "matrix");
	m_owlObjectTypePropertyObject = GetPropertyByName((int64_t)ifcObject, "object");
	m_owlObjectTypePropertyObjects = GetPropertyByName((int64_t)ifcObject, "objects");
}

IFC_MATERIAL* IFCConvector::CreateMaterial()
{
	IFC_MATERIAL* material = new IFC_MATERIAL;

	material->ambient.R = -1;
	material->ambient.G = -1;
	material->ambient.B = -1;
	material->ambient.A = -1;

	material->diffuse.R = -1;
	material->diffuse.G = -1;
	material->diffuse.B = -1;
	material->diffuse.A = -1;

	material->specular.R = -1;
	material->specular.G = -1;
	material->specular.B = -1;
	material->specular.A = -1;

	material->emissive.R = -1;
	material->emissive.G = -1;
	material->emissive.B = -1;
	material->emissive.A = -1;

	material->transparency = -1;
	material->shininess = -1;

	material->next = 0;
	material->prev = m_lastMaterial;

	if (m_firstMaterial == 0) {
		//ASSERT(lastMaterial == 0);
		m_firstMaterial = material;
	}
	m_lastMaterial = material;
	if (m_lastMaterial->prev) {
		m_lastMaterial->prev->next = m_lastMaterial;
	}

	//material->MTRL = nullptr;
	material->active = false;

	return	material;
}

IFC_MATERIALS* IFCConvector::CreateMaterials(IFC_MATERIAL * material)
{
	IFC_MATERIALS	* materials = new IFC_MATERIALS;

	materials->material = material;
	materials->next = nullptr;

	materials->__indexOffsetForFaces = 0;
	materials->__indexArrayOffset = 0;

	materials->__noPrimitivesForFaces = 0;
	materials->__indexBufferSize = 0;

	return	materials;
}

IFC_MATERIALS* IFCConvector::CreateMaterials(IFC_MATERIAL * material,int_t ifcModel,int_t ifcInstance)
{
	IFC_MATERIALS	* materials = CreateMaterials(material);
	materials->__noPrimitivesForFaces = 0;
	materials->__indexBufferSize = -1;

	return	materials;
}

IFC_MATERIAL_META_INFO* IFCConvector::CreateMaterialMetaInfo(int_t ifcInstance)
{
	IFC_MATERIAL_META_INFO	* materialMetaInfo = new IFC_MATERIAL_META_INFO;
	//new_matMI++;

	if (ifcInstance) {
		materialMetaInfo->expressID = internalGetP21Line(ifcInstance);
	}
	else {
		materialMetaInfo->expressID = -1;
	}

	materialMetaInfo->isPoint = false;
	materialMetaInfo->isEdge = false;
	materialMetaInfo->isShell = false;

	materialMetaInfo->material = CreateMaterial();

	materialMetaInfo->child = 0;
	materialMetaInfo->next = 0;

	return	materialMetaInfo;
}

void IFCConvector::ClearMaterials()
{
	IFC_MATERIAL	* currentMaterial = m_firstMaterial;

	while (currentMaterial) {
		IFC_MATERIAL	* materialToDelete = NULL;
		if (currentMaterial != m_defaultMaterial) {
			materialToDelete = currentMaterial;
		}
		currentMaterial = currentMaterial->next;
		if (materialToDelete) {
			delete  materialToDelete;
			materialToDelete = NULL;
		}
	}
	if (m_defaultMaterial) {
		delete  m_defaultMaterial;
	}

	m_firstMaterial = NULL;
	m_lastMaterial = NULL;
	m_defaultMaterial = NULL;
}

void IFCConvector::DeleteMaterial(IFC_MATERIAL * material)
{
	if (material) {
		//totalAllocatedMaterial--;

		//ASSERT(material->active == false);

		//ASSERT(DEBUG_colorLoopConsistanceCheck());

		IFC_MATERIAL	* prev = material->prev,
			*next = material->next;

		if (prev == 0) {
			//ASSERT(firstMaterial == next->prev);
		}

		if (next == 0) {
			//ASSERT(lastMaterial == prev->next);
		}

		if (prev) {
			//ASSERT(prev->next == material);
			prev->next = next;
		}
		else {
			//ASSERT(firstMaterial == material);
			m_firstMaterial = next;
			next->prev = 0;
		}

		if (next) {
			//ASSERT(next->prev == material);
			next->prev = prev;
		}
		else {
			//ASSERT(lastMaterial == material);
			m_lastMaterial = prev;
			//ASSERT(prev->next == 0);
		}

		material->active = false;

		delete	material;
	}
}

void IFCConvector::DeleteMaterialMetaInfo(IFC_MATERIAL_META_INFO * materialMetaInfo)
{
	while (materialMetaInfo) {
		DeleteMaterialMetaInfo(materialMetaInfo->child);

		IFC_MATERIAL_META_INFO	* materialMetaInfoToDelete = materialMetaInfo;

		materialMetaInfo = materialMetaInfo->next;

		//del_matMI++;
		delete	materialMetaInfoToDelete;
	}
}

void IFCConvector::MinimizeMaterialItems(IFC_MATERIAL_META_INFO * materialMetaInfo, IFC_MATERIAL ** ppMaterial, bool * pUnique, bool * pDefaultColorIsUsed)
{
	while (materialMetaInfo) {
		//
		//	Check nested child object (i.e. Mapped Items)
		//
		if (materialMetaInfo->child) {
			//ASSERT(materialMetaInfo->material->ambient.R == -1);
			//ASSERT(materialMetaInfo->material->active == false);
			DeleteMaterial(materialMetaInfo->material);
			materialMetaInfo->material = 0;
			MinimizeMaterialItems(materialMetaInfo->child, ppMaterial, pUnique, pDefaultColorIsUsed);
		}

		//
		//	Complete Color
		//
		IFC_MATERIAL	* material = materialMetaInfo->material;
		if (material) {
			if (material->ambient.R == -1) {
				materialMetaInfo->material = m_defaultMaterial;
				DeleteMaterial(material);
			}
			else {
				if (material->diffuse.R == -1) {
					material->diffuse.R = material->ambient.R;
					material->diffuse.G = material->ambient.G;
					material->diffuse.B = material->ambient.B;
					material->diffuse.A = material->ambient.A;
				}
				if (material->specular.R == -1) {
					material->specular.R = material->ambient.R;
					material->specular.G = material->ambient.G;
					material->specular.B = material->ambient.B;
					material->specular.A = material->ambient.A;
				}
			}
		}
		else {
			//ASSERT(materialMetaInfo->child);
		}

		//ASSERT(DEBUG_colorLoopConsistanceCheck());

		//
		//	Merge the same colors
		//
		material = materialMetaInfo->material;
		if (material  &&  material != m_defaultMaterial) {
			bool	adjusted = false;
			//ASSERT(material->active == false);
			IFC_MATERIAL	* materialLoop = m_firstMaterial->next;
			//ASSERT(firstMaterial == defaultMaterial);
			while (materialLoop) {
				if ((materialLoop->active) &&
					(material->transparency == materialLoop->transparency) &&
					(material->ambient.R == materialLoop->ambient.R) &&
					(material->ambient.G == materialLoop->ambient.G) &&
					(material->ambient.B == materialLoop->ambient.B) &&
					(material->ambient.A == materialLoop->ambient.A) &&
					(material->diffuse.R == materialLoop->diffuse.R) &&
					(material->diffuse.G == materialLoop->diffuse.G) &&
					(material->diffuse.B == materialLoop->diffuse.B) &&
					(material->diffuse.A == materialLoop->diffuse.A) &&
					(material->specular.R == materialLoop->specular.R) &&
					(material->specular.G == materialLoop->specular.G) &&
					(material->specular.B == materialLoop->specular.B) &&
					(material->specular.A == materialLoop->specular.A)) {
						materialMetaInfo->material = materialLoop;
						DeleteMaterial(material);
						materialLoop = 0;
						adjusted = true;
				}
				else {
					if (materialLoop->active == false) {
						while (materialLoop) {
							//ASSERT(materialLoop->active == false);
							materialLoop = materialLoop->next;
						}
						materialLoop = 0;
					}
					else {
						materialLoop = materialLoop->next;
					}
				}
			}

			if (adjusted) {
				//ASSERT(materialMetaInfo->material->active);
			}
			else {
				//ASSERT(materialMetaInfo->material->active == false);
				materialMetaInfo->material->active = true;
			}

			//ASSERT(materialLoop == 0 || (materialLoop == defaultMaterial  &&  materialLoop->next == 0));
		}

		//
		//	Assign default color in case of no color and no children
		//
		if (materialMetaInfo->material == 0 && materialMetaInfo->child == 0) {
			materialMetaInfo->material = m_defaultMaterial;
		}

		//
		//	Check if unique
		//
		material = materialMetaInfo->material;
		if ((*ppMaterial)) {
			if ((*ppMaterial) != material) {
				if (material == 0 && materialMetaInfo->child) {
				}
				else {
					(*pUnique) = false;
				}
			}
		}
		else {
			//ASSERT((*pUnique) == true);
			(*ppMaterial) = material;
		}

		if (material == m_defaultMaterial) {
			(*pDefaultColorIsUsed) = true;
		}

		materialMetaInfo = materialMetaInfo->next;
	}
}

void IFCConvector::GetRGB_productDefinitionShape(int_t model, int_t ifcObjectInstance, IFC_MATERIAL_META_INFO ** ppMaterialMetaInfo)
{
	int_t	* representations_set = nullptr, representations_cnt, i = 0;
	sdaiGetAttrBN(ifcObjectInstance, (char*)L"Representations", sdaiAGGR, &representations_set);
	representations_cnt = sdaiGetMemberCount(representations_set);
	while (i < representations_cnt) {
		int_t	ifcShapeRepresentation = 0;
		engiGetAggrElement(representations_set, i, sdaiINSTANCE, &ifcShapeRepresentation);
		if (ifcShapeRepresentation != 0) {
			GetRGB_shapeRepresentation(
				model,
				ifcShapeRepresentation,
				ppMaterialMetaInfo
				);
		}
		i++;
	}
}

void IFCConvector::GetRGB_shapeRepresentation(int_t model, int_t ifcShapeRepresentationInstance, IFC_MATERIAL_META_INFO ** ppMaterialMetaInfo)
{
	wchar_t * pRepresentationIdentifier = nullptr, *RepresentationType = nullptr;
	sdaiGetAttrBN(ifcShapeRepresentationInstance, (char*)L"RepresentationIdentifier", sdaiUNICODE, &pRepresentationIdentifier);
	sdaiGetAttrBN(ifcShapeRepresentationInstance, (char*)L"RepresentationType", sdaiUNICODE, &RepresentationType);

	if ((__equals(pRepresentationIdentifier, L"Body") || __equals(pRepresentationIdentifier, L"Mesh") || __equals(pRepresentationIdentifier, L"Facetation")) &&
		!__equals(RepresentationType, L"BoundingBox"))
	{
		int_t	* geometry_set = nullptr, geometry_cnt, i = 0;
		sdaiGetAttrBN(ifcShapeRepresentationInstance, (char*)L"Items", sdaiAGGR, &geometry_set);
		geometry_cnt = sdaiGetMemberCount(geometry_set);
		i = 0;
		while (i < geometry_cnt) {
			int_t	geometryInstance = 0, styledItemInstance = 0;
			engiGetAggrElement(geometry_set, i, sdaiINSTANCE, &geometryInstance);

			//ASSERT((*ppMaterialMetaInfo) == 0);
			(*ppMaterialMetaInfo) = CreateMaterialMetaInfo(geometryInstance);

			sdaiGetAttrBN(geometryInstance, (char*)L"StyledByItem", sdaiINSTANCE, &styledItemInstance);
			if (styledItemInstance != 0)
			{
				GetRGB_styledItem(
#ifdef _DEBUG
					model,
#endif
					styledItemInstance,
					(*ppMaterialMetaInfo)->material
					);
			}
			else {
				SearchDeeper(model, geometryInstance, ppMaterialMetaInfo, (*ppMaterialMetaInfo)->material);
			}

			ppMaterialMetaInfo = &(*ppMaterialMetaInfo)->next;
			i++;
		}
	}
}

void IFCConvector::GetRGB_styledItem(
#ifdef _DEBUG 
	int_t model, 
#endif 
	int_t ifcStyledItemInstance, 
	IFC_MATERIAL * material )
{
	int_t	* styles_set = nullptr, styles_cnt, i = 0;
	sdaiGetAttrBN(ifcStyledItemInstance, (char*)L"Styles", sdaiAGGR, &styles_set);
	styles_cnt = sdaiGetMemberCount(styles_set);
	while (i < styles_cnt) {
		int_t presentationStyleAssignmentInstance = 0;
		engiGetAggrElement(styles_set, i, sdaiINSTANCE, &presentationStyleAssignmentInstance);
		if (presentationStyleAssignmentInstance != 0) {
			GetRGB_presentationStyleAssignment(
#ifdef _DEBUG
				model,
#endif
				presentationStyleAssignmentInstance,
				material
				);
		}
		i++;
	}
}

void IFCConvector::GetRGB_presentationStyleAssignment(
#ifdef _DEBUG 
	int_t model, 
#endif 
	int_t presentationStyleAssignmentInstance, 
	IFC_MATERIAL * material )
{
	int_t	* styles_set = nullptr, styles_cnt, i = 0;
	sdaiGetAttrBN(presentationStyleAssignmentInstance, (char*)L"Styles", sdaiAGGR, &styles_set);
	styles_cnt = sdaiGetMemberCount(styles_set);
	while (i < styles_cnt) {
		int_t	surfaceStyleInstance = 0;
		engiGetAggrElement(styles_set, i, sdaiINSTANCE, &surfaceStyleInstance);
		if (surfaceStyleInstance != 0) {
			GetRGB_surfaceStyle(
#ifdef _DEBUG
				model,
#endif
				surfaceStyleInstance,
				material
				);
		}
		i++;
	}
}

void IFCConvector::GetRGB_surfaceStyle(
#ifdef _DEBUG 
	int_t model, 
#endif 
	int_t surfaceStyleInstance, 
	IFC_MATERIAL * material )
{
	int_t	* styles_set = nullptr, styles_cnt;
	sdaiGetAttrBN(surfaceStyleInstance, (char*)L"Styles", sdaiAGGR, &styles_set);
	styles_cnt = sdaiGetMemberCount(styles_set);
	for (int_t i = 0; i < styles_cnt; i++) {
		int_t	surfaceStyleRenderingInstance = 0, SurfaceColourInstance = 0, SpecularColourInstance = 0, DiffuseColourInstance = 0;
		engiGetAggrElement(styles_set, i, sdaiINSTANCE, &surfaceStyleRenderingInstance);
#ifdef _DEBUG
		int_t	surfaceStyleRenderingEntity = sdaiGetEntity(model, (char*)L"IFCSURFACESTYLERENDERING");
#endif
		//ASSERT(sdaiGetInstanceType(surfaceStyleRenderingInstance) == surfaceStyleRenderingEntity);

		double	transparency = 0;
		sdaiGetAttrBN(surfaceStyleRenderingInstance, (char*)L"Transparency", sdaiREAL, &transparency);
		material->transparency = 1 - (float)transparency;

		sdaiGetAttrBN(surfaceStyleRenderingInstance, (char*)L"SurfaceColour", sdaiINSTANCE, &SurfaceColourInstance);
		if (SurfaceColourInstance != 0) {
			GetRGB_colourRGB(SurfaceColourInstance, &material->ambient);
		}
		else {
			//ASSERT(false);
		}

		sdaiGetAttrBN(surfaceStyleRenderingInstance, (char*)L"DiffuseColour", sdaiINSTANCE, &DiffuseColourInstance);
		if (DiffuseColourInstance != 0) {
			GetRGB_colourRGB(DiffuseColourInstance, &material->diffuse);
		}
		else {
			int_t	ADB = 0;
			sdaiGetAttrBN(surfaceStyleRenderingInstance, (char*)L"DiffuseColour", sdaiADB, &ADB);
			if (ADB) {
				double	value = 0;
				sdaiGetADBValue((void *)ADB, sdaiREAL, &value);
				material->diffuse.R = (float)value * material->ambient.R;
				material->diffuse.G = (float)value * material->ambient.G;
				material->diffuse.B = (float)value * material->ambient.B;
			}
		}

		sdaiGetAttrBN(surfaceStyleRenderingInstance, (char*)L"SpecularColour", sdaiINSTANCE, &SpecularColourInstance);
		if (SpecularColourInstance != 0) {
			GetRGB_colourRGB(SpecularColourInstance, &material->specular);
		}
		else {
			int_t	ADB = 0;
			sdaiGetAttrBN(surfaceStyleRenderingInstance, (char*)L"SpecularColour", sdaiADB, &ADB);
			if (ADB) {
				double	value = 0;
				sdaiGetADBValue((void *)ADB, sdaiREAL, &value);
				material->specular.R = (float)value * material->ambient.R;
				material->specular.G = (float)value * material->ambient.G;
				material->specular.B = (float)value * material->ambient.B;
			}
		}
	}
}

void IFCConvector::GetRGB_colourRGB(int_t SurfaceColourInstance, IFC_COLOR * color)
{
	double	red = 0, green = 0, blue = 0;
	sdaiGetAttrBN(SurfaceColourInstance, (char*)L"Red", sdaiREAL, &red);
	sdaiGetAttrBN(SurfaceColourInstance, (char*)L"Green", sdaiREAL, &green);
	sdaiGetAttrBN(SurfaceColourInstance, (char*)L"Blue", sdaiREAL, &blue);

	color->R = (float)red;
	color->G = (float)green;
	color->B = (float)blue;
}

void IFCConvector::GetRGB_relAssociatesMaterial(int_t model, int_t ifcRelAssociatesMaterialInstance, IFC_MATERIAL * material)
{
	int_t	ifcMaterialEntity = sdaiGetEntity(model, (char*)L"IFCMATERIAL"),
		ifcMaterialLayerSetUsageEntity = sdaiGetEntity(model, (char*)L"IFCMATERIALLAYERSETUSAGE"),
		ifcMaterialLayerSetEntity = sdaiGetEntity(model, (char*)L"IFCMATERIALLAYERSET"),
		ifcMaterialLayerEntity = sdaiGetEntity(model, (char*)L"IFCMATERIALLAYER"),
		ifcMaterialSelectInstance = 0;
	sdaiGetAttrBN(ifcRelAssociatesMaterialInstance, (char*)L"RelatingMaterial", sdaiINSTANCE, &ifcMaterialSelectInstance);

	if (sdaiGetInstanceType(ifcMaterialSelectInstance) == ifcMaterialEntity) {
		GetRGB_ifcMaterial(model, ifcMaterialSelectInstance, material);
	}
	else if (sdaiGetInstanceType(ifcMaterialSelectInstance) == ifcMaterialLayerSetUsageEntity) {
		GetRGB_ifcMaterialLayerSetUsage(model, ifcMaterialSelectInstance, material);
	}
	else if (sdaiGetInstanceType(ifcMaterialSelectInstance) == ifcMaterialLayerSetEntity) {
		GetRGB_ifcMaterialLayerSet(model, ifcMaterialSelectInstance, material);
	}
	else if (sdaiGetInstanceType(ifcMaterialSelectInstance) == ifcMaterialLayerEntity) {
		GetRGB_ifcMaterialLayer(model, ifcMaterialSelectInstance, material);
	}
}

void IFCConvector::GetRGB_ifcMaterial(int_t model, int_t ifcMaterialInstance, IFC_MATERIAL * material)
{
	int_t	* ifcMaterialDefinitionRepresentationAggr = nullptr,
		ifcMaterialDefinitionRepresentationAggrCnt = 0,
		i = 0;
	sdaiGetAttrBN(ifcMaterialInstance, (char*)L"HasRepresentation", sdaiAGGR, &ifcMaterialDefinitionRepresentationAggr);
	ifcMaterialDefinitionRepresentationAggrCnt = sdaiGetMemberCount(ifcMaterialDefinitionRepresentationAggr);
	while (i < ifcMaterialDefinitionRepresentationAggrCnt) {
		int_t	ifcMaterialDefinitionRepresentationInstance = 0;
		engiGetAggrElement(ifcMaterialDefinitionRepresentationAggr, i, sdaiINSTANCE, &ifcMaterialDefinitionRepresentationInstance);

		if (sdaiGetInstanceType(ifcMaterialDefinitionRepresentationInstance) == sdaiGetEntity(model, (char*)L"IFCMATERIALDEFINITIONREPRESENTATION")) {
			GetRGB_ifcMaterialDefinitionRepresentation(model, ifcMaterialDefinitionRepresentationInstance, material);
		}
		else {
			//ASSERT(false);
		}
		i++;
	}
}

void IFCConvector::GetRGB_ifcMaterialDefinitionRepresentation(int_t model, int_t ifcMaterialDefinitionRepresentationInstance, IFC_MATERIAL * material)
{
	int_t	* ifcRepresentationAggr = nullptr,
		ifcRepresentationAggrCnt = 0,
		i = 0;
	sdaiGetAttrBN(ifcMaterialDefinitionRepresentationInstance, (char*)L"Representations", sdaiAGGR, &ifcRepresentationAggr);
	ifcRepresentationAggrCnt = sdaiGetMemberCount(ifcRepresentationAggr);
	while (i < ifcRepresentationAggrCnt) {
		int_t	ifcRepresentationInstance = 0;
		engiGetAggrElement(ifcRepresentationAggr, i, sdaiINSTANCE, &ifcRepresentationInstance);

		if (sdaiGetInstanceType(ifcRepresentationInstance) == sdaiGetEntity(model, (char*)L"IFCSTYLEDREPRESENTATION")) {
			GetRGB_ifcStyledRepresentation(model, ifcRepresentationInstance, material);
		}
		i++;
	}
}

void IFCConvector::GetRGB_ifcStyledRepresentation(int_t model, int_t ifcStyledRepresentationInstance, IFC_MATERIAL * material)
{
	int_t	* ifcRepresentationItemAggr = nullptr,
		ifcRepresentationItemAggrCnt = 0,
		i = 0;
	sdaiGetAttrBN(ifcStyledRepresentationInstance, (char*)L"Items", sdaiAGGR, &ifcRepresentationItemAggr);
	ifcRepresentationItemAggrCnt = sdaiGetMemberCount(ifcRepresentationItemAggr);
	while (i < ifcRepresentationItemAggrCnt) {
		int_t	ifcRepresentationItemInstance = 0;
		engiGetAggrElement(ifcRepresentationItemAggr, i, sdaiINSTANCE, &ifcRepresentationItemInstance);

		if (sdaiGetInstanceType(ifcRepresentationItemInstance) == sdaiGetEntity(model, (char*)L"IFCSTYLEDITEM")) {
			GetRGB_styledItem(
#ifdef _DEBUG
				model,
#endif
				ifcRepresentationItemInstance,
				material
				);
		}
		i++;
	}
}

void IFCConvector::GetRGB_ifcMaterialLayerSetUsage(int_t model, int_t ifcMaterialLayerSetUsage, IFC_MATERIAL * material)
{
	int_t	ifcMaterialLayerSetInstance = 0;
	sdaiGetAttrBN(ifcMaterialLayerSetUsage, (char*)L"ForLayerSet", sdaiINSTANCE, &ifcMaterialLayerSetInstance);

	//ASSERT(sdaiGetInstanceType(ifcMaterialLayerSetInstance) == sdaiGetEntity(model, (char*)L"IFCMATERIALLAYERSET"));
	GetRGB_ifcMaterialLayerSet(model, ifcMaterialLayerSetInstance, material);
}

void IFCConvector::GetRGB_ifcMaterialLayerSet(int_t model, int_t ifcMaterialLayerSet, IFC_MATERIAL * material)
{
	int_t	* ifcMaterialLayerAggr = nullptr,
		ifcMaterialLayerAggrCnt = 0;
	sdaiGetAttrBN(ifcMaterialLayerSet, (char*)L"MaterialLayers", sdaiAGGR, &ifcMaterialLayerAggr);
	ifcMaterialLayerAggrCnt = sdaiGetMemberCount(ifcMaterialLayerAggr);
	if (ifcMaterialLayerAggrCnt) {
		int_t	ifcMaterialLayer = 0;
		engiGetAggrElement(ifcMaterialLayerAggr, 0, sdaiINSTANCE, &ifcMaterialLayer);

		if (sdaiGetInstanceType(ifcMaterialLayer) == sdaiGetEntity(model, (char*)L"IFCMATERIALLAYER")) {
			GetRGB_ifcMaterialLayer(model, ifcMaterialLayer, material);
		}
		else {
			//ASSERT(false);
		}
	}
}

void IFCConvector::GetRGB_ifcMaterialLayer(int_t model, int_t ifcMaterialLayer, IFC_MATERIAL * material)
{
	int_t	ifcMaterialInstance = 0;
	sdaiGetAttrBN(ifcMaterialLayer, (char*)L"Material", sdaiINSTANCE, &ifcMaterialInstance);

	//ASSERT(sdaiGetInstanceType(ifcMaterialInstance) == sdaiGetEntity(model, (char*)L"IFCMATERIAL"));
	GetRGB_ifcMaterial(model, ifcMaterialInstance, material);
}

void IFCConvector::WalkThroughGeometry__transformation(int64_t owlModel, int64_t owlInstance, IFC_MATERIALS *** ppMaterials, IFC_MATERIAL_META_INFO ** ppMaterialMetaInfo)
{
	//ASSERT(GetInstanceClass(owlInstance) == rdfClassTransformation);

	int64_t	* owlInstanceObject = 0, objectCard = 0;
	GetObjectTypeProperty(owlInstance, m_owlObjectTypePropertyObject, &owlInstanceObject, &objectCard);
	if (objectCard == 1) {
		WalkThroughGeometry__collection(owlModel, owlInstanceObject[0], ppMaterials, ppMaterialMetaInfo);
	}
	else {
		//ASSERT(false);
	}
}

void IFCConvector::WalkThroughGeometry__collection(int64_t owlModel, int64_t owlInstance, IFC_MATERIALS *** ppMaterials, IFC_MATERIAL_META_INFO ** ppMaterialMetaInfo)
{
	if (GetInstanceClass(owlInstance) == m_rdfClassCollection) {
		int64_t	* owlInstanceObjects = 0, objectsCard = 0;
		GetObjectTypeProperty(owlInstance, m_owlObjectTypePropertyObjects, &owlInstanceObjects, &objectsCard);
		for (int_t i = 0; i < objectsCard; i++) {
			WalkThroughGeometry__object(owlModel, owlInstanceObjects[i], ppMaterials, ppMaterialMetaInfo);
		}
	}
	else {
		WalkThroughGeometry__object(owlModel, owlInstance, ppMaterials, ppMaterialMetaInfo);
	}
}

void IFCConvector::WalkThroughGeometry__object(int64_t owlModel, int64_t owlInstance, IFC_MATERIALS *** ppMaterials, IFC_MATERIAL_META_INFO ** ppMaterialMetaInfo)
{
	int64_t	* owlInstanceExpressID = 0, expressIDCard = 0;
	GetDataTypeProperty(owlInstance, m_owlDataTypePropertyExpressID, (void **)&owlInstanceExpressID, &expressIDCard);
	if (expressIDCard == 1) {
		int64_t	expressID = owlInstanceExpressID[0];
		while ((*ppMaterialMetaInfo) && (*ppMaterialMetaInfo)->expressID != expressID) {
			ppMaterialMetaInfo = &(*ppMaterialMetaInfo)->next;
		}
		if ((*ppMaterialMetaInfo)) {
			//ASSERT((*ppMaterialMetaInfo)->expressID == expressID);
			if ((*ppMaterialMetaInfo)->child) {
				IFC_MATERIAL_META_INFO	** ppMaterialMetaInfoChild = &(*ppMaterialMetaInfo)->child;
				if (GetInstanceClass(owlInstance) == m_rdfClassTransformation) {
					int64_t	* owlInstanceObject = 0, objectCard = 0;
					GetObjectTypeProperty(owlInstance, m_owlObjectTypePropertyObject, &owlInstanceObject, &objectCard);
					if (objectCard == 1) {
						WalkThroughGeometry__object(owlModel, owlInstanceObject[0], ppMaterials, ppMaterialMetaInfoChild);
					}
					else {
						//ASSERT(false);
					}
				}
				else if (GetInstanceClass(owlInstance) == m_rdfClassCollection) {
					int64_t	* owlInstanceObjects = 0, objectsCard = 0;
					GetObjectTypeProperty(owlInstance, m_owlObjectTypePropertyObjects, &owlInstanceObjects, &objectsCard);
					for (int_t i = 0; i < objectsCard; i++) {
						WalkThroughGeometry__object(owlModel, owlInstanceObjects[i], ppMaterials, ppMaterialMetaInfoChild);
					}
				}
				else {
					//ASSERT(false);
				}
			}
			else {
				//
				//	Now recreate the geometry for this object
				//
				int64_t	vertexBufferSize = 0, indexBufferSize = 0;
				CalculateInstance(owlInstance, &vertexBufferSize, &indexBufferSize, 0);

				if ((**ppMaterials)) {
					(*ppMaterials) = &(**ppMaterials)->next;
				}
				(**ppMaterials) = CreateMaterials((*ppMaterialMetaInfo)->material);
				(**ppMaterials)->__indexBufferSize = indexBufferSize;
			}
			ppMaterialMetaInfo = &(*ppMaterialMetaInfo)->next;
		}
		else {
			//ASSERT(false);
		}
	}
	else {
		if (GetInstanceClass(owlInstance) == m_rdfClassTransformation) {
			int64_t	* owlInstanceObject = 0, objectCard = 0;
			GetObjectTypeProperty(owlInstance, m_owlObjectTypePropertyObject, &owlInstanceObject, &objectCard);

			if (objectCard == 1) {
				WalkThroughGeometry__object(owlModel, owlInstanceObject[0], ppMaterials, ppMaterialMetaInfo);
			}
			else {
				//ASSERT(false);
			}
		}
		else if (GetInstanceClass(owlInstance) == m_rdfClassCollection) {
			int64_t	* owlInstanceObjects = 0, objectsCard = 0;
			GetObjectTypeProperty(owlInstance, m_owlObjectTypePropertyObjects, &owlInstanceObjects, &objectsCard);
			int_t	i = 0;
			while (i < objectsCard) {
				WalkThroughGeometry__object(owlModel, owlInstanceObjects[i], ppMaterials, ppMaterialMetaInfo);
				i++;
			}
		}
		else {
			//ASSERT(false);
		}
	}
}

void IFCConvector::SearchDeeper(int_t model, int_t geometryInstance, IFC_MATERIAL_META_INFO ** ppMaterialMetaInfo, IFC_MATERIAL * material)
{
	int_t	styledItemInstance = 0;
	sdaiGetAttrBN(geometryInstance, (char*)L"StyledByItem", sdaiINSTANCE, &styledItemInstance);
	if (styledItemInstance != 0) {
		GetRGB_styledItem(
#ifdef _DEBUG
			model,
#endif
			styledItemInstance,
			material
			);
		if (material->ambient.R >= 0) {
			return;
		}
	}

	int_t	booleanClippingResultEntity = sdaiGetEntity(model, (char*)L"IFCBOOLEANCLIPPINGRESULT"),
		booleanResultEntity = sdaiGetEntity(model, (char*)L"IFCBOOLEANRESULT"),
		mappedItemEntity = sdaiGetEntity(model, (char*)L"IFCMAPPEDITEM"),
		shellBasedSurfaceModelEntity = sdaiGetEntity(model, (char*)L"IFCSHELLBASEDSURFACEMODEL");

	if (sdaiGetInstanceType(geometryInstance) == booleanResultEntity || sdaiGetInstanceType(geometryInstance) == booleanClippingResultEntity) {
		int_t	geometryChildInstance = 0;
		sdaiGetAttrBN(geometryInstance, (char*)L"FirstOperand", sdaiINSTANCE, &geometryChildInstance);
		if (geometryChildInstance) {
			SearchDeeper(model, geometryChildInstance, ppMaterialMetaInfo, material);
		}
	}
	else if (sdaiGetInstanceType(geometryInstance) == mappedItemEntity) {
		int_t	representationMapInstance = 0;
		sdaiGetAttrBN(geometryInstance, (char*)L"MappingSource", sdaiINSTANCE, &representationMapInstance);
		int_t	shapeRepresentationInstance = 0;
		sdaiGetAttrBN(representationMapInstance, (char*)L"MappedRepresentation", sdaiINSTANCE, &shapeRepresentationInstance);

		if (shapeRepresentationInstance) {
			//ASSERT((*ppMaterialMetaInfo)->child == 0);
			GetRGB_shapeRepresentation(model, shapeRepresentationInstance, &(*ppMaterialMetaInfo)->child);
		}
	}
	else if (sdaiGetInstanceType(geometryInstance) == shellBasedSurfaceModelEntity) {
		int_t	* geometryChildAggr = nullptr;
		sdaiGetAttrBN(geometryInstance, (char*)L"SbsmBoundary", sdaiAGGR, &geometryChildAggr);
		IFC_MATERIAL_META_INFO	** ppSubMaterialMetaInfo = &(*ppMaterialMetaInfo)->child;

		int_t	cnt = sdaiGetMemberCount(geometryChildAggr), i = 0;
		while (i < cnt) {
			int_t	geometryChildInstance = 0;
			engiGetAggrElement(geometryChildAggr, i, sdaiINSTANCE, &geometryChildInstance);
			if (geometryChildInstance) {
				//ASSERT((*ppSubMaterialMetaInfo) == 0);
				(*ppSubMaterialMetaInfo) = CreateMaterialMetaInfo(geometryChildInstance);

				SearchDeeper(model, geometryChildInstance, ppSubMaterialMetaInfo, (*ppSubMaterialMetaInfo)->material);
				ppSubMaterialMetaInfo = &(*ppSubMaterialMetaInfo)->next;
			}
			i++;
		}
	}
}

//---------------------------------------------------------------------------------------------
// brief 属性
void IFCConvector::IFCPropToFiled(OgdcRecordset* pRecordset, IFC__PROPERTY__SET* pIFCPropSet)
{
	IFC__PROPERTY__SET* pIFCPropSetNext = pIFCPropSet;

	while (pIFCPropSetNext)
	{
		IFC__PROPERTY* pProperty = pIFCPropSetNext->properties;
		while (pProperty)
		{
			wchar_t* name = pProperty->name;

			UGString strName = UGString(pIFCPropSetNext->name) + _U("_") + UGString(name);
			UGuint nLength = strName.GetLength();
			UGString strPropName = strName;
			if (nLength > 20)
			{
				strPropName = m_propForeign[strName];
			}
			std::set<UGString>::iterator it = m_propList.find(strName);
			if (it != m_propList.end())
			{
				if (pProperty->nominalValue)
				{
					pRecordset->SetFieldValue(strPropName, UGVariant(pProperty->nominalValue));
				}
				else if (pProperty->volumeValue)
				{
					pRecordset->SetFieldValue(strPropName, UGVariant(pProperty->nominalValue));
				}
				else if (pProperty->timeValue)
				{
					pRecordset->SetFieldValue(strPropName, UGVariant(pProperty->timeValue));
				}
				else if (pProperty->lengthValue)
				{
					pRecordset->SetFieldValue(strPropName, UGVariant(pProperty->lengthValue));
				}
				else if (pProperty->countValue)
				{
					pRecordset->SetFieldValue(strPropName, UGVariant(pProperty->countValue));
				}
				else if (pProperty->areaValue)
				{
					pRecordset->SetFieldValue(strPropName, UGVariant(pProperty->areaValue));
				}

				if (pProperty->unit)
				{
					UGString strUnit = strPropName + _U("_Unit");
					pRecordset->SetFieldValue(strUnit, UGVariant(pProperty->unit));
				}
			}

			pProperty = pProperty->next;
		}

		pIFCPropSetNext = pIFCPropSetNext->next;
	}
}

void IFCConvector::ClearPropertys()
{
	std::map<int_t, IFC__PROPERTY__SET*>::iterator it = m_ifcProperty.begin();
	while (it!= m_ifcProperty.end())
	{
		DeletePropertySet(it->second);
		it++;
	}

	m_ifcProperty.clear();
	m_propList.clear();
}

void IFCConvector::DeletePropertySet(IFC__PROPERTY__SET* ifcPorpSet)
{
	while (ifcPorpSet) {
		IFC__PROPERTY__SET	* pSetToRemove = ifcPorpSet;
		IFC__PROPERTY		* properties = ifcPorpSet->properties;
		while (properties) {
			IFC__PROPERTY	* pToRemove = properties;
			properties = properties->next;

			if (pToRemove->name) {
				delete pToRemove->name;
			}
			if (pToRemove->description) {
				delete pToRemove->description;
			}
			if (pToRemove->nominalValue) {
				delete pToRemove->nominalValue;
			}
			if (pToRemove->lengthValue) {
				delete pToRemove->lengthValue;
			}
			if (pToRemove->areaValue) {
				delete pToRemove->areaValue;
			}
			if (pToRemove->countValue) {
				delete pToRemove->countValue;
			}
			if (pToRemove->timeValue) {
				delete pToRemove->timeValue;
			}
			if (pToRemove->volumeValue) {
				delete pToRemove->volumeValue;
			}
			if (pToRemove->weigthValue) {
				delete pToRemove->weigthValue;
			}
			//			delete pToRemove->nameBuffer;
			delete pToRemove;
		}
		ifcPorpSet = ifcPorpSet->next;

		if (pSetToRemove->name) {
			delete[] pSetToRemove->name;
		}
		if (pSetToRemove->description) {
			delete[] pSetToRemove->description;
		}
		delete pSetToRemove;
	}
}

void IFCConvector::CreateObjProList(int_t ifcInstance, int_t entity , int_t & index, OgdcRecordset* pRecordset /*= NULL*/)
{
	if (entity)
	{
		CreateObjProList(ifcInstance, engiGetEntityParent(entity), index, pRecordset);

		wchar_t	* entityName = nullptr;
		engiGetEntityName(entity, sdaiUNICODE, (char**)&entityName);

		int_t	argCnt = engiGetEntityNoArguments(entity);
		while (index < argCnt) {
			wchar_t	* propertyName = nullptr;
			engiGetEntityArgumentName(entity, index, sdaiUNICODE, (char**)&propertyName);

			int_t	propertyType = 0;
			engiGetEntityArgumentType(entity, index, &propertyType);

			UGString strName = UGString(propertyName);
			if (pRecordset == NULL)
			{
				m_objPropList.insert(strName);
			}
			else
			{
				UGString value = NestedPropertyValue(ifcInstance, propertyName, propertyType, 0);

				//QMessageBox::warning(NULL, "", QString::fromWCharArray(value));

				pRecordset->SetFieldValue(UGString(strName), value);
			}
			index++;
		}

		entity = engiGetEntityParent(entity);
	}
}

void IFCConvector::CreateIfcInstanceProperties(int_t ifcModel, IFC__PROPERTY__SET ** propertySets, int_t ifcObjectInstance, IFC__SIUNIT * units)
{
	int_t		* isDefinedByInstances = 0,
		ifcRelDefinesByType_TYPE = sdaiGetEntity(ifcModel, (char*)L"IFCRELDEFINESBYTYPE"),
		ifcRelDefinesByProperties_TYPE = sdaiGetEntity(ifcModel, (char*)L"IFCRELDEFINESBYPROPERTIES");

	sdaiGetAttrBN(ifcObjectInstance, (char*)L"IsDefinedBy", sdaiAGGR, &isDefinedByInstances);

	if (isDefinedByInstances) {
		int_t	isDefinedByInstancesCnt = sdaiGetMemberCount(isDefinedByInstances);
		for (int_t i = 0; i < isDefinedByInstancesCnt; ++i) {
			int_t	isDefinedByInstance = 0;
			engiGetAggrElement(isDefinedByInstances, i, sdaiINSTANCE, &isDefinedByInstance);

			if (sdaiGetInstanceType(isDefinedByInstance) == ifcRelDefinesByType_TYPE) {
				int_t	typeObjectInstance = 0;

				sdaiGetAttrBN(isDefinedByInstance, (char*)L"RelatingType", sdaiINSTANCE, &typeObjectInstance);

				CreateTypeObjectInstance(ifcModel, propertySets, typeObjectInstance, units);
			}
			else if (sdaiGetInstanceType(isDefinedByInstance) == ifcRelDefinesByProperties_TYPE) {
				CreateRelDefinesByProperties(ifcModel, propertySets, isDefinedByInstance, units);
			}
			else {
				//ASSERT(false);
			}
		}
	}
}

void IFCConvector::CreateRelDefinesByProperties(int_t ifcModel, IFC__PROPERTY__SET ** propertySets, int_t ifcRelDefinesByProperties, IFC__SIUNIT * units)
{
	if (ifcRelDefinesByProperties) {
		int_t	ifcPropertySetInstance = 0,
			ifcElementQuantity_TYPE = sdaiGetEntity(ifcModel, (char*)L"IFCELEMENTQUANTITY"),
			ifcPropertySet_TYPE = sdaiGetEntity(ifcModel, (char*)L"IFCPROPERTYSET");

		sdaiGetAttrBN(ifcRelDefinesByProperties, (char*)L"RelatingPropertyDefinition", sdaiINSTANCE, &ifcPropertySetInstance);

		if (sdaiGetInstanceType(ifcPropertySetInstance) == ifcElementQuantity_TYPE) {
			CreateIfcElementQuantity(ifcModel, propertySets, ifcPropertySetInstance, units);
		}
		else if (sdaiGetInstanceType(ifcPropertySetInstance) == ifcPropertySet_TYPE) {
			CreateIfcPropertySet(ifcModel, propertySets, ifcPropertySetInstance, units);
		}
		else {
			//ASSERT(false);
		}
	}
	else {
		//ASSERT(false);
	}
}

void IFCConvector::CreateTypeObjectInstance(int_t ifcModel, IFC__PROPERTY__SET ** propertySets, int_t ifcTypeObjectInstance, IFC__SIUNIT * units)
{
	if (ifcTypeObjectInstance) {
		int_t	* hasPropertySets = 0, hasPropertySetsCnt,
			ifcElementQuantity_TYPE = sdaiGetEntity(ifcModel, (char*)L"IFCELEMENTQUANTITY"),
			ifcPropertySet_TYPE = sdaiGetEntity(ifcModel, (char*)L"IFCPROPERTYSET");

		sdaiGetAttrBN(ifcTypeObjectInstance, (char*)L"HasPropertySets", sdaiAGGR, &hasPropertySets);

		hasPropertySetsCnt = sdaiGetMemberCount(hasPropertySets);
		for (int_t i = 0; i < hasPropertySetsCnt; ++i) {
			int_t	hasPropertySetInstance = 0;
			engiGetAggrElement(hasPropertySets, i, sdaiINSTANCE, &hasPropertySetInstance);
			if (sdaiGetInstanceType(hasPropertySetInstance) == ifcElementQuantity_TYPE) {
				CreateIfcElementQuantity(ifcModel, propertySets, hasPropertySetInstance, units);
			}
			else if (sdaiGetInstanceType(hasPropertySetInstance) == ifcPropertySet_TYPE) {
				CreateIfcPropertySet(ifcModel, propertySets, hasPropertySetInstance, units);
			}
			else {
				//ASSERT(false);
			}
		}
	}
}

void IFCConvector::CreateIfcElementQuantity(int_t ifcModel, IFC__PROPERTY__SET ** propertySets, int_t ifcPropertySetInstance, IFC__SIUNIT * units)
{
	IFC__PROPERTY__SET	** ppPropertySet = propertySets;//&ifcObject->propertySets;
	while ((*ppPropertySet)) {
		ppPropertySet = &(*ppPropertySet)->next;
	}

	wchar_t	* name = 0, *description = 0;
	sdaiGetAttrBN(ifcPropertySetInstance, (char*)L"Name", sdaiUNICODE, &name);
	sdaiGetAttrBN(ifcPropertySetInstance, (char*)L"Description", sdaiUNICODE, &description);

	//ASSERT((*ppPropertySet) == 0);
	(*ppPropertySet) = CreateIfcPropertySet(ifcPropertySetInstance, name, description);
	IFC__PROPERTY	** ppProperty = &(*ppPropertySet)->properties;

	int_t	* ifcQuantityInstances = 0;
#ifdef	_DEBUG
	//	int_t	ifcRelDefinesByType_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCRELDEFINESBYTYPE"),
	//			ifcRelDefinesByProperties_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCRELDEFINESBYPROPERTIES");
#endif

	sdaiGetAttrBN(ifcPropertySetInstance, (char*)L"Quantities", sdaiAGGR, &ifcQuantityInstances);

	if (ifcQuantityInstances) {
		int_t	ifcQuantityInstancesCnt = sdaiGetMemberCount(ifcQuantityInstances);
		for (int_t i = 0; i < ifcQuantityInstancesCnt; ++i) {
			int_t	ifcQuantityInstance = 0,
				ifcQuantityLength_TYPE = sdaiGetEntity(ifcModel, (char*)L"IFCQUANTITYLENGTH"),
				ifcQuantityArea_TYPE = sdaiGetEntity(ifcModel, (char*)L"IFCQUANTITYAREA"),
				ifcQuantityVolume_TYPE = sdaiGetEntity(ifcModel, (char*)L"IFCQUANTITYVOLUME"),
				ifcQuantityCount_TYPE = sdaiGetEntity(ifcModel, (char*)L"IFCQUANTITYCOUNT"),
				ifcQuantityWeigth_TYPE = sdaiGetEntity(ifcModel, (char*)L"IFCQUANTITYWEIGHT"),
				ifcQuantityTime_TYPE = sdaiGetEntity(ifcModel, (char*)L"IFCQUANTITYTIME");

			engiGetAggrElement(ifcQuantityInstances, i, sdaiINSTANCE, &ifcQuantityInstance);

			wchar_t	* quantityName = 0, *quantityDescription = 0;
			sdaiGetAttrBN(ifcQuantityInstance, (char*)L"Name", sdaiUNICODE, &quantityName);
			sdaiGetAttrBN(ifcQuantityInstance, (char*)L"Description", sdaiUNICODE, &quantityDescription);

			//ASSERT((*ppProperty) == 0);
			(*ppProperty) = CreateIfcProperty(ifcQuantityInstance, quantityName, quantityDescription);

			// 添加到属性列表
			UGString propName = UGString(name) + _U("_") +UGString(quantityName);
			m_propList.insert(propName);

			if (sdaiGetInstanceType(ifcQuantityInstance) == ifcQuantityLength_TYPE) {
				CreateIfcQuantityLength(ifcQuantityInstance, (*ppProperty), units);
			}
			else if (sdaiGetInstanceType(ifcQuantityInstance) == ifcQuantityArea_TYPE) {
				CreateIfcQuantityArea(ifcQuantityInstance, (*ppProperty), units);
			}
			else if (sdaiGetInstanceType(ifcQuantityInstance) == ifcQuantityVolume_TYPE) {
				CreateIfcQuantityVolume(ifcQuantityInstance, (*ppProperty), units);
			}
			else if (sdaiGetInstanceType(ifcQuantityInstance) == ifcQuantityCount_TYPE) {
				CreateIfcQuantityCount(ifcQuantityInstance, (*ppProperty));//, units);
			}
			else if (sdaiGetInstanceType(ifcQuantityInstance) == ifcQuantityWeigth_TYPE) {
				CreateIfcQuantityWeigth(ifcQuantityInstance, (*ppProperty), units);
			}
			else if (sdaiGetInstanceType(ifcQuantityInstance) == ifcQuantityTime_TYPE) {
				CreateIfcQuantityTime(ifcQuantityInstance, (*ppProperty), units);
			}
			else {
				//ASSERT(false);
			}

			ppProperty = &(*ppProperty)->next;
		}
	}
}

IFC__PROPERTY__SET	* IFCConvector::CreateIfcPropertySet(int_t ifcInstance, wchar_t * name, wchar_t * description)
{
	IFC__PROPERTY__SET	* ifcPropertySet = new IFC__PROPERTY__SET;

	ifcPropertySet->structType = STRUCT_TYPE_PROPERTY_SET;
	ifcPropertySet->ifcInstance = ifcInstance;
	ifcPropertySet->name = copyStr(name);
	ifcPropertySet->description = copyStr(description);

	ifcPropertySet->nameBuffer = new wchar_t[512];

	ifcPropertySet->properties = NULL;
	ifcPropertySet->next = NULL;

	return	ifcPropertySet;
}

void IFCConvector::CreateIfcPropertySet(int_t ifcModel, IFC__PROPERTY__SET ** propertySets, int_t ifcPropertySetInstance, IFC__SIUNIT * units)
{
	IFC__PROPERTY__SET	** ppPropertySet = propertySets;//&ifcObject->propertySets;
	while ((*ppPropertySet)) {
		ppPropertySet = &(*ppPropertySet)->next;
	}

	wchar_t	* name = 0, *description = 0;
	sdaiGetAttrBN(ifcPropertySetInstance, (char*)L"Name", sdaiUNICODE, &name);
	sdaiGetAttrBN(ifcPropertySetInstance, (char*)L"Description", sdaiUNICODE, &description);

	//ASSERT((*ppPropertySet) == 0);
	(*ppPropertySet) = CreateIfcPropertySet(ifcPropertySetInstance, name, description);
	IFC__PROPERTY	** ppProperty = &(*ppPropertySet)->properties;

	int_t	* ifcPropertyInstances = 0;
#ifdef	_DEBUG
	//	int_t	ifcRelDefinesByType_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCRELDEFINESBYTYPE"),
	//			ifcRelDefinesByProperties_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCRELDEFINESBYPROPERTIES");
#endif

	sdaiGetAttrBN(ifcPropertySetInstance, (char*)L"HasProperties", sdaiAGGR, &ifcPropertyInstances);

	if (ifcPropertyInstances) {
		int_t	ifcPropertyInstancesCnt = sdaiGetMemberCount(ifcPropertyInstances);
		for (int_t i = 0; i < ifcPropertyInstancesCnt; ++i) {
			int_t	ifcPropertyInstance = 0,
				ifcPropertySingleValue_TYPE = sdaiGetEntity(ifcModel, (char*)L"IFCPROPERTYSINGLEVALUE");

			engiGetAggrElement(ifcPropertyInstances, i, sdaiINSTANCE, &ifcPropertyInstance);

			wchar_t	* propertyName = 0, *propertyDescription = 0;
			sdaiGetAttrBN(ifcPropertyInstance, (char*)L"Name", sdaiUNICODE, &propertyName);
			sdaiGetAttrBN(ifcPropertyInstance, (char*)L"Description", sdaiUNICODE, &propertyDescription);

			//ASSERT((*ppProperty) == 0);
			(*ppProperty) = CreateIfcProperty(ifcPropertyInstance, propertyName, propertyDescription);

			// 添加到属性列表
			UGString propName = UGString(name) + _U("_") + UGString(propertyName);
			m_propList.insert(propName);

			if (sdaiGetInstanceType(ifcPropertyInstance) == ifcPropertySingleValue_TYPE) {
				CreateIfcPropertySingleValue(ifcPropertyInstance, (*ppProperty), units);
			}
			else {
				//				ASSERT(false);
			}

			ppProperty = &(*ppProperty)->next;
		}
	}
}

void IFCConvector::CreateIfcPropertySingleValue(int_t ifcPropertySingleValue, IFC__PROPERTY * ifcProperty, IFC__SIUNIT * units)
{
	wchar_t	* nominalValue = 0,
		*unit = 0,
		*typePath = 0;
	int_t	* nominalValueADB = 0;

	sdaiGetAttrBN(ifcPropertySingleValue, (char*)L"NominalValue", sdaiUNICODE, &nominalValue);
	if (nominalValue) {
		sdaiGetAttrBN(ifcPropertySingleValue, (char*)L"NominalValue", sdaiADB, &nominalValueADB);
		typePath = (wchar_t*)sdaiGetADBTypePath(nominalValueADB, 0);
		sdaiGetAttrBN(ifcPropertySingleValue, (char*)L"Unit", sdaiUNICODE, &unit);

		if (unit == 0) {
			if (equalStr(typePath, L"IFCBOOLEAN")) {
			}
			else if (equalStr(typePath, L"IFCIDENTIFIER")) {
			}
			else if (equalStr(typePath, L"IFCINTEGER")) {
			}
			else if (equalStr(typePath, L"IFCLABEL")) {
			}
			else if (equalStr(typePath, L"IFCLOGICAL")) {
			}
			else if (equalStr(typePath, L"IFCTEXT")) {
			}
			else if (equalStr(typePath, L"IFCREAL")) {
			}
			else if (equalStr(typePath, L"IFCCOUNTMEASURE")) {
			}
			else if (equalStr(typePath, L"IFCPOSITIVERATIOMEASURE")) {
			}
			else if (equalStr(typePath, L"IFCVOLUMETRICFLOWRATEMEASURE")) {
			}
			else if (equalStr(typePath, L"IFCABSORBEDDOSEMEASURE")) {
				unit = GetUnit(units, L"ABSORBEDDOSEUNIT");
			}
			else if (equalStr(typePath, L"IFCAMOUNTOFSUBSTANCEMEASURE")) {
				unit = GetUnit(units, L"AMOUNTOFSUBSTANCEUNIT");
			}
			else if (equalStr(typePath, L"IFCAREAMEASURE")) {
				unit = GetUnit(units, L"AREAUNIT");
			}
			else if (equalStr(typePath, L"IFCDOSEEQUIVALENTMEASURE")) {
				unit = GetUnit(units, L"DOSEEQUIVALENTUNIT");
			}
			else if (equalStr(typePath, L"IFCELECTRICCAPACITANCEMEASURE")) {
				unit = GetUnit(units, L"ELECTRICCAPACITANCEUNIT");
			}
			else if (equalStr(typePath, L"IFCELECTRICCHARGEMEASURE")) {
				unit = GetUnit(units, L"ELECTRICCHARGEUNIT");
			}
			else if (equalStr(typePath, L"IFCELECTRICCONDUCTANCEMEASURE")) {
				unit = GetUnit(units, L"ELECTRICCONDUCTANCEUNIT");
			}
			else if (equalStr(typePath, L"IFCELECTRICCURRENTMEASURE")) {
				unit = GetUnit(units, L"ELECTRICCURRENTUNIT");
			}
			else if (equalStr(typePath, L"IFCELECTRICRESISTANCEMEASURE")) {
				unit = GetUnit(units, L"ELECTRICRESISTANCEUNIT");
			}
			else if (equalStr(typePath, L"IFCELECTRICVOLTAGEMEASURE")) {
				unit = GetUnit(units, L"ELECTRICVOLTAGEUNIT");
			}
			else if (equalStr(typePath, L"IFCENERGYMEASURE")) {
				unit = GetUnit(units, L"ENERGYUNIT");
			}
			else if (equalStr(typePath, L"IFCFORCEMEASURE")) {
				unit = GetUnit(units, L"FORCEUNIT");
			}
			else if (equalStr(typePath, L"IFCFREQUENCYMEASURE")) {
				unit = GetUnit(units, L"FREQUENCYUNIT");
			}
			else if (equalStr(typePath, L"IFCILLUMINANCEMEASURE")) {
				unit = GetUnit(units, L"ILLUMINANCEUNIT");
			}
			else if (equalStr(typePath, L"IFCINDUCTANCEMEASURE")) {
				unit = GetUnit(units, L"INDUCTANCEUNIT");
			}
			else if (equalStr(typePath, L"IFCLENGTHMEASURE") || equalStr(typePath, L"IFCPOSITIVELENGTHMEASURE")) {
				unit = GetUnit(units, L"LENGTHUNIT");
			}
			else if (equalStr(typePath, L"IFCLUMINOUSFLUXMEASURE")) {
				unit = GetUnit(units, L"LUMINOUSFLUXUNIT");
			}
			else if (equalStr(typePath, L"IFCLUMINOUSINTENSITYMEASURE")) {
				unit = GetUnit(units, L"LUMINOUSINTENSITYUNIT");
			}
			else if (equalStr(typePath, L"IFCMAGNETICFLUXDENSITYMEASURE")) {
				unit = GetUnit(units, L"MAGNETICFLUXDENSITYUNIT");
			}
			else if (equalStr(typePath, L"IFCMAGNETICFLUXMEASURE")) {
				unit = GetUnit(units, L"MAGNETICFLUXUNIT");
			}
			else if (equalStr(typePath, L"IFCMASSMEASURE")) {
				unit = GetUnit(units, L"MASSUNIT");
			}
			else if (equalStr(typePath, L"IFCPLANEANGLEMEASURE")) {
				unit = GetUnit(units, L"PLANEANGLEUNIT");
			}
			else if (equalStr(typePath, L"IFPOWERCMEASURE")) {
				unit = GetUnit(units, L"POWERUNIT");
			}
			else if (equalStr(typePath, L"IFCPRESSUREMEASURE")) {
				unit = GetUnit(units, L"PRESSUREUNIT");
			}
			else if (equalStr(typePath, L"IFCRADIOACTIVITYMEASURE")) {
				unit = GetUnit(units, L"RADIOACTIVITYUNIT");
			}
			else if (equalStr(typePath, L"IFCSOLIDANGLEMEASURE")) {
				unit = GetUnit(units, L"SOLIDANGLEUNIT");
			}
			else if (equalStr(typePath, L"IFCTHERMODYNAMICTEMPERATUREMEASURE")) {
				unit = GetUnit(units, L"THERMODYNAMICTEMPERATUREUNIT");
			}
			else if (equalStr(typePath, L"IFCTIMEMEASURE")) {
				unit = GetUnit(units, L"TIMEUNIT");
			}
			else if (equalStr(typePath, L"IFCVOLUMEMEASURE")) {
				unit = GetUnit(units, L"VOLUMEUNIT");
			}
			else if (equalStr(typePath, L"IFCUSERDEFINEDMEASURE")) {
				unit = GetUnit(units, L"USERDEFINED");
			}
			else if (equalStr(typePath, L"IFCTHERMALTRANSMITTANCEMEASURE")) {
				unit = GetUnit(units, L"???");
			}
			else {
				//ASSERT(false);
			}
		}
		else {
			//ASSERT(false);
		}
		ifcProperty->nominalValue = copyStr(nominalValue);
		ifcProperty->unit = copyStr(unit);
	}
	else {
		ifcProperty->nominalValue = 0;
		ifcProperty->unit = 0;
	}
}

IFC__PROPERTY	* IFCConvector::CreateIfcProperty(int_t ifcInstance, wchar_t * name, wchar_t * description)
{
	IFC__PROPERTY	* ifcProperty = new IFC__PROPERTY;

	ifcProperty->structType = STRUCT_TYPE_PROPERTY;
	ifcProperty->ifcInstance = ifcInstance;
	ifcProperty->name = copyStr(name);
	ifcProperty->description = copyStr(description);

	ifcProperty->nominalValue = NULL;
	ifcProperty->lengthValue  = NULL;
	ifcProperty->areaValue    = NULL;
	ifcProperty->volumeValue  = NULL;
	ifcProperty->countValue   = NULL;
	ifcProperty->weigthValue  = NULL;
	ifcProperty->timeValue    = NULL;
	ifcProperty->unit         = NULL;

	//ifcProperty->hTreeItem = 0;
	ifcProperty->nameBuffer = new wchar_t[512 + 1];
	ifcProperty->next = NULL;

	return	ifcProperty;
}

void IFCConvector::CreateIfcQuantityLength(int_t ifcQuantityLength, IFC__PROPERTY * ifcProperty, IFC__SIUNIT * units)
{
	wchar_t	* lengthValue = NULL;
	wchar_t	* unit = NULL;
	int_t	ifcUnitInstance = NULL;

	sdaiGetAttrBN(ifcQuantityLength, (char*)L"LengthValue", sdaiUNICODE, &lengthValue);
	sdaiGetAttrBN(ifcQuantityLength, (char*)L"Unit", sdaiUNICODE, &unit);
	sdaiGetAttrBN(ifcQuantityLength, (char*)L"Unit", sdaiINSTANCE, &ifcUnitInstance);

	//ASSERT(ifcUnitInstance == 0);

	ifcProperty->lengthValue = copyStr(lengthValue);
	ifcProperty->unit = copyStr(unit);
	if (unit == NULL || unit[0] == NULL) {
		while (units) {
			if (units->type == LENGTHUNIT) {
				ifcProperty->unit = units->name;
			}
			units = units->next;
		}
	}
}

void IFCConvector::CreateIfcQuantityArea(int_t ifcQuantityArea, IFC__PROPERTY * ifcProperty, IFC__SIUNIT * units)
{
	wchar_t	* areaValue = NULL;
	wchar_t	* unit = NULL;

	sdaiGetAttrBN(ifcQuantityArea, (char*)L"AreaValue", sdaiUNICODE, &areaValue);
	sdaiGetAttrBN(ifcQuantityArea, (char*)L"Unit", sdaiUNICODE, &unit);

	ifcProperty->areaValue = copyStr(areaValue);
	ifcProperty->unit = copyStr(unit);
	if (unit == NULL || unit[0] == NULL) {
		while (units) {
			if (units->type == AREAUNIT) {
				ifcProperty->unit = units->name;
			}
			units = units->next;
		}
	}
}

void IFCConvector::CreateIfcQuantityVolume(int_t ifcQuantityVolume, IFC__PROPERTY * ifcProperty, IFC__SIUNIT * units)
{
	wchar_t	* volumeValue = NULL;
	wchar_t	* unit = NULL;

	sdaiGetAttrBN(ifcQuantityVolume, (char*)L"VolumeValue", sdaiUNICODE, &volumeValue);
	sdaiGetAttrBN(ifcQuantityVolume, (char*)L"Unit", sdaiUNICODE, &unit);

	ifcProperty->volumeValue = copyStr(volumeValue);
	ifcProperty->unit = copyStr(unit);
	if (unit == NULL || unit[0] == NULL) {
		while (units) {
			if (units->type == VOLUMEUNIT) {
				ifcProperty->unit = units->name;
			}
			units = units->next;
		}
	}
}

void IFCConvector::CreateIfcQuantityCount(int_t ifcQuantityCount, IFC__PROPERTY * ifcProperty)
{
	wchar_t	* countValue = NULL;
	wchar_t	* unit = NULL;

	sdaiGetAttrBN(ifcQuantityCount, (char*)L"CountValue", sdaiUNICODE, &countValue);
	sdaiGetAttrBN(ifcQuantityCount, (char*)L"Unit", sdaiUNICODE, &unit);

	ifcProperty->countValue = copyStr(countValue);
	ifcProperty->unit = copyStr(unit);
}

void IFCConvector::CreateIfcQuantityWeigth(int_t ifcQuantityWeigth, IFC__PROPERTY * ifcProperty, IFC__SIUNIT * units)
{
	wchar_t	* weigthValue = NULL;
	wchar_t	* unit = NULL;

	sdaiGetAttrBN(ifcQuantityWeigth, (char*)L"WeigthValue", sdaiUNICODE, &weigthValue);
	sdaiGetAttrBN(ifcQuantityWeigth, (char*)L"Unit", sdaiUNICODE, &unit);

	ifcProperty->weigthValue = copyStr(weigthValue);
	ifcProperty->unit = copyStr(unit);
	if (unit == NULL || unit[0] == NULL) {
		while (units) {
			if (units->type == MASSUNIT) {
				ifcProperty->unit = units->name;
			}
			units = units->next;
		}
	}
}

void IFCConvector::CreateIfcQuantityTime(int_t ifcQuantityTime, IFC__PROPERTY * ifcProperty, IFC__SIUNIT * units)
{
	wchar_t	* timeValue = NULL;
	wchar_t	* unit = NULL;

	sdaiGetAttrBN(ifcQuantityTime, (char*)L"TimeValue", sdaiUNICODE, &timeValue);
	sdaiGetAttrBN(ifcQuantityTime, (char*)L"Unit", sdaiUNICODE, &unit);

	ifcProperty->timeValue = copyStr(timeValue);
	ifcProperty->unit = copyStr(unit);
	if (unit == NULL || unit[0] == NULL) {
		while (units) {
			if (units->type == TIMEUNIT) {
				ifcProperty->unit = units->name;
			}
			units = units->next;
		}
	}
}

OGDC::OgdcString IFCConvector::NestedPropertyValue(int_t ifcEntityInstance, wchar_t * propertyName, int_t propertyType, int_t * ifcAggregate)
{
	UGString rvalue = _U("");

	if (ifcAggregate) {
		engiGetAggrType(ifcAggregate, &propertyType);
	}
	else {
		if (ifcEntityInstance == 0 || propertyName == NULL || propertyType == 0)
		{
			return rvalue;
		}
	}

	wchar_t	* buffer = nullptr, buff[512];

	int_t	ifcInstance, lineNo, *ifcInstanceAggr, ifcInstanceAggrCnt, integer;

	wchar_t	* nominalValue = nullptr,
		*typePath = nullptr;
	int_t	* nominalValueADB = nullptr;

	switch (propertyType) {
	case  sdaiADB:
		sdaiGetAttrBN(ifcEntityInstance, (char*)propertyName, sdaiUNICODE, &nominalValue);
		if (nominalValue) {
			sdaiGetAttrBN(ifcEntityInstance, (char*)propertyName, sdaiADB, &nominalValueADB);
			typePath = (wchar_t*)sdaiGetADBTypePath(nominalValueADB, sdaiUNICODE);

			UGString value(typePath);

			rvalue += value;
		}
		break;
	case  sdaiAGGR:
		ifcInstanceAggr = 0;
		if (ifcEntityInstance) {
			sdaiGetAttrBN(ifcEntityInstance, (char *)propertyName, sdaiAGGR, &ifcInstanceAggr);
		}
		else {
			engiGetAggrElement(ifcAggregate, 0, sdaiAGGR, &ifcInstanceAggr);
		}
		if (ifcInstanceAggr) {
			ifcInstanceAggrCnt = sdaiGetMemberCount(ifcInstanceAggr);

			if (ifcInstanceAggrCnt > 0) {
				//rValue += NestedPropertyValue(0, 0, 0, ifcInstanceAggr);
				rvalue += NestedPropertyValue(0, 0, 0, ifcInstanceAggr);
				if (ifcInstanceAggrCnt > 1) {
					//rValue += ", ...";
				}
			}
			else {
				//ASSERT(ifcInstanceAggrCnt == 0);
				//rValue += " ?";
			}
		}
		else {
			//rValue += "???";
		}
		//rValue += ")";
		break;
	case  sdaiBINARY:
	case  sdaiBOOLEAN:
	case  sdaiENUM:
		if (ifcEntityInstance) {
			sdaiGetAttrBN(ifcEntityInstance, (char*)propertyName, sdaiUNICODE, &buffer);
		}
		else {
			engiGetAggrElement(ifcAggregate, 0, sdaiUNICODE, &buffer);
		}

		if (buffer  &&  buffer[0]) {
			//rValue += buffer;
			rvalue += UGString(buffer);
		}
		else {
			//rValue += "?";
		}
		break;
	case  sdaiINSTANCE:
		if (ifcEntityInstance) {
			sdaiGetAttrBN(ifcEntityInstance, (char *)propertyName, sdaiINSTANCE, &ifcInstance);
		}
		else {
			engiGetAggrElement(ifcAggregate, 0, sdaiINSTANCE, &ifcInstance);
		}
		if (ifcInstance) {
			lineNo = internalGetP21Line(ifcInstance);
			rvalue += _U("#");
#ifdef	WIN64
			_i64tow_s(lineNo, buff, 500, 10);
#else
			_itow_s(lineNo, buff, 10);
#endif		
			rvalue += UGString(buff);		
		}
		else {
			//rValue += "?";
		}
		break;
	case  sdaiINTEGER:
		integer = 0;
		if (ifcEntityInstance) {
			sdaiGetAttrBN(ifcEntityInstance, (char *)propertyName, sdaiINTEGER, &integer);
		}
		else {
			engiGetAggrElement(ifcAggregate, 0, sdaiINTEGER, &integer);
		}
#ifdef	WIN64
		_i64tow_s(integer, buff, 500, 10);
#else
		_itow_s(integer, buff, 10);
#endif

		rvalue += UGString(buff);
		break;
	case  sdaiLOGICAL:
	case  sdaiREAL:
		if (ifcEntityInstance) {
			sdaiGetAttrBN(ifcEntityInstance, (char*)propertyName, sdaiUNICODE, &buffer);
		}
		else {
			engiGetAggrElement(ifcAggregate, 0, sdaiUNICODE, &buffer);
		}

		if (buffer  &&  buffer[0]) {
			rvalue += UGString(buff);
		}
		else {
			//rValue += "?";
		}
		break;
	case  sdaiSTRING:
		if (ifcEntityInstance) {
			sdaiGetAttrBN(ifcEntityInstance, (char*)propertyName, sdaiUNICODE, &buffer);
		}
		else {
			engiGetAggrElement(ifcAggregate, 0, sdaiUNICODE, &buffer);
		}

		rvalue += UGString(buffer);

		//rValue += '\'';
		//rValue += buffer;
		//rValue += '\'';
		break;
	default:
		//rValue += "<NOT IMPLEMENTED>";
		break;
	}

	return rvalue;
}
