/**
 * @file ExtraHelp.h
 * @author Connor Szabo
 * @date Oct 2019
 * @brief File containing function definitions to help parse GPX files
**/
#include "GPXParser.h"

/** This function is to help parse the xml tree created in GPXParser.c
 * @pre GPXdoc exists but is empty
 * @post GPXDoc exists and is filled with information from xml file
 * @param pointer to the GPXDoc and XML file node pointer
**/
void parse(GPXdoc *parsed, xmlNode * root);

/** This functions calculates distance using the haversine formula
 * @pre All of the parameters are valid numbers
 * @post Returns the distance 
 * @param Latitude and Longitude (both floats) for 2 different points
**/
float distance(float lat1, float lon1, float lat2, float lon2);

void wDocToXml(xmlDoc *newDoc, GPXdoc *doc);

void vDocToXml(xmlDoc *newDoc, GPXdoc *doc);

/**
	Returns false
	using for if validation error is found
**/
bool err();