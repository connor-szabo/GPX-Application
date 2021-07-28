//Connor Szabo
//0954113
//szabo@uoguelph.ca

#include "GPXParser.h"
#include "ExtraHelp.h"

GPXdoc* createGPXdoc(char* filename){

	//The xml document being parsed
	xmlDoc *doc = NULL;
	//The root of the xml tree
	xmlNode *root = NULL;
	
	LIBXML_TEST_VERSION
	
	//Parse the xml document into a tree
	doc = xmlReadFile(filename, NULL, 0);
	
	if(doc == NULL){
		return NULL;
	}
	
	//The GPX document to be created from the xml file
	GPXdoc *parsed = malloc(sizeof(GPXdoc));
	
	//Initialize waypoint List
	parsed->waypoints = initializeList(waypointToString, deleteWaypoint, compareWaypoints);

	
	//Initialize Route List
	parsed->routes = initializeList(routeToString, deleteRoute, compareRoutes);
	
	//Initialize Tracks List
	parsed->tracks = initializeList(trackToString, deleteTrack, compareTracks);
	
	//Get the head node of the xml tree
	root = xmlDocGetRootElement(doc);
	
	if(root == NULL){
		return NULL;
	}
	
	//fill in the GPXdoc with information from the xml tree
	parse(parsed, root);
	
	xmlMemoryDump();
	xmlCleanupParser();


	return parsed;
}

char* GPXdocToString(GPXdoc* doc){
	size_t size = snprintf(NULL, 0, "Namespace: %s\nVersion: %lf\nCreator: %s\n", doc->namespace, doc->version, doc->creator);
	char* holder = malloc(size+1);
	sprintf(holder, "Namespace: %s\nVersion: %lf\nCreator: %s\n", doc->namespace, doc->version, doc->creator);
	
	List* wpt = doc->waypoints;
	List* rte = doc->routes;
	List* trk = doc->tracks;
	
	
	char* wpts = toString(wpt);
	char* rtes = toString(rte);
	char* trks = toString(trk);

	
	size = snprintf(NULL, 0, "%s %s", holder, wpts);
	holder = realloc(holder, size+1);
	strcat(holder, wpts);
	
	size = snprintf(NULL, 0, "%s %s", holder, rtes);
	holder = realloc(holder, size+1);
	strcat(holder, rtes);
	
	size = snprintf(NULL, 0, "%s %s", holder, trks);
	holder = realloc(holder, size+1);
	strcat(holder, trks);
	
	free(rtes);
	free(wpts);
	free(trks);
	
	return holder;
}

void deleteGPXdoc(GPXdoc* doc){
	freeList(doc->tracks);
	freeList(doc->routes);
	freeList(doc->waypoints);
	free(doc->creator);
	free(doc->namespace);
}

/******************Attributes******************/

void deleteAttribute(void* data){
	Attribute* delete = (Attribute*)data;
	free(delete->name);
	free(delete->value);
}

char* attributeToString(void* data){
	Attribute * att = (Attribute*)data;
	size_t size = snprintf(NULL, 0, "Attribute Name: %s\nAttribute Value: %s\n", att->name, att->value);
	char *holder = malloc(size+1);
	sprintf(holder, "Attribute Name: %s\nAttribute Value: %s\n", att->name, att->value);
	return holder;

}

int compareAttributes(const void* first, const void* second){
	Attribute * a = (Attribute*)first;
	Attribute * b = (Attribute*)second;
	
	if(a == b){
		return 0;
	}
	else{
		return 1;
	}

}

/******************Waypoint******************/

void deleteWaypoint(void* data){
	Waypoint* delete = (Waypoint*) data;
	free(delete->name);
	freeList(delete->attributes);	
}

char* waypointToString(void* data){
	Waypoint *wpt = (Waypoint*) data;
	size_t size = snprintf(NULL, 0, "Waypoint Name: %s\n\tLat: %lf\n\tLon: %lf\n", wpt->name, wpt->latitude, wpt->longitude);
	
	char* holder = malloc(size + 1);
	sprintf(holder, "Waypoint Name: %s\n\tLat: %lf\n\tLon: %lf\n", wpt->name, wpt->latitude, wpt->longitude);
	
	//Get a string of all of the waypoints attributes	
	List* list = wpt->attributes;
	char* atts = toString(list);
	
	size = snprintf(NULL, 0, "%s %s", holder, atts);
	
	holder = realloc(holder, size + 1);
	
	strcat(holder, atts);
	
	free(atts);
	
	return holder;
	
}

int compareWaypoints(const void *first, const void *second){
	Waypoint * a = (Waypoint*)first;
	Waypoint * b = (Waypoint*)second;
	
	if(a == b){
		return 0;
	}
	else{
		return 1;
	}
}

/******************Route******************/

void deleteRoute(void* data){
	Route* rte = (Route*) data;
	freeList(rte->attributes);
	freeList(rte->waypoints);
	free(rte->name);
}

char* routeToString(void* data){
	Route* rte = (Route*)data;
	
	size_t size = snprintf(NULL, 0, "Route Name: %s\n", rte->name);
	char* holder = malloc(size+1);
	sprintf(holder, "Route Name: %s\n", rte->name);
	
	List* wpt = rte->waypoints;
	List* att = rte->attributes;
	
	char* atts = toString(att);
	char* wpts = toString(wpt);
	
	size = snprintf(NULL, 0, "%s %s", atts, wpts);
	
	atts = realloc(atts, size+1);
	
	strcat(atts, wpts);
	free(wpts);
	
	size = snprintf(NULL, 0, "%s %s", atts, holder);
	holder = realloc(holder, size+1);
	strcat(holder, atts);


	free(atts);
	
	return holder;
	
}

int compareRoutes(const void *first, const void *second){
	Route * a = (Route*)first;
	Route * b = (Route*)second;
	if(a == b){
		return 0;
	}
	else{
		return 1;
	}

}

/******************Track Segment******************/

void deleteTrackSegment(void* data){
	TrackSegment* delete = (TrackSegment*) data;
	freeList(delete->waypoints);
}

char* trackSegmentToString(void* data){
	TrackSegment * temp = (TrackSegment*)data;
	
	List *list = temp->waypoints;
	
	char * holder = malloc(sizeof("Track Segment\n"));
	strcpy(holder, "Track Segment\n");
	char* wpts = toString(list);
	size_t size = snprintf(NULL, 0, "%s %s", holder, wpts);
	
	holder = realloc(holder, size+1);
	strcat(holder, wpts);
	
	free(wpts);
	
	return holder;
	
}

int compareTrackSegments(const void *first, const void *second){
	TrackSegment * a = (TrackSegment*)first;
	TrackSegment * b = (TrackSegment*)second;
	
	if(a == b){
		return 0;
	}
	else{
		return 1;
	}
}

/******************Track******************/

void deleteTrack(void* data){
	Track* delete = (Track*) data;
	freeList(delete->segments);
	freeList(delete->attributes);
	free(delete->name);
}

char* trackToString(void* data){
	Track* trk = (Track*)data;
	
	size_t size = snprintf(NULL, 0, "Track Name: %s\n", trk->name);
	char* holder = malloc(size+1);
	sprintf(holder, "Track Name: %s\n", trk->name);
	
	List* trkseg = trk->segments;
	List* att = trk->attributes;
	
	char* atts = toString(att);
	char* trksegs = toString(trkseg);
	
	size = snprintf(NULL, 0, "%s %s", holder, atts);
	holder = realloc(holder, size+1);
	
	strcat(holder, atts);
	
	size = snprintf(NULL, 0, "%s %s", holder, trksegs);
	holder = realloc(holder, size+1);
	
	strcat(holder, trksegs);
	
	free(atts);
	free(trksegs);
	
	return holder;
	

}

int compareTracks(const void *first, const void *second){
	Track * a = (Track*)first;
	Track * b = (Track*)second;
	
	if(a == b){
		return 0;
	}
	else{
		return 1;
	}
}

//**************** Getters ******************
int getNumWaypoints(const GPXdoc* doc){
	int i = 0;
	if(doc == NULL){
		return i;
	}
	if(doc->waypoints == NULL){
		return i;
	}
	List *list = doc->waypoints;
	if(list->head == NULL){
		return i;
	}
	Node* itter = list->head;
	
	while(itter){
		i++;
		itter = itter->next;
	}
	return i;
}

int getNumRoutes(const GPXdoc* doc){
	int i = 0;
	if(doc == NULL){
		return i;
	}
	else if(doc->routes == NULL){
		return i;
	}
	List *list = doc->routes;
	if(list->head == NULL){
		return i;
	}
	Node *itter = doc->routes->head;
	
	while(itter){
		i++;
		itter = itter->next;
	}
	
	return i;
}

//Number of tracks in the GPX file
int getNumTracks(const GPXdoc* doc){
	int i = 0;
	//Check if the gpx doc is Null
	if(doc == NULL){
		return i;
	}
	//Check if tracks are Null
	if(doc->tracks == NULL){
		return i;
	}
	
	List* list = doc->tracks;
	//Check if Tracks is initialized but null
	if(list->head == NULL){
		return i;
	}
	
	Node *itter = list->head;
	
	while(itter){
		i++;
		itter = itter->next;
	}
	
	return i;
}

//Total number of segments in all tracks in the document
int getNumSegments(const GPXdoc* doc){
	if(doc == NULL){
		return 0;
	}
	int num = 0;
	List* list = doc->tracks;
	Node* node = list->head;
	
	while(node){
		Track* trk = node->data;
		num = num + trk->segments->length;
		node = node->next;
	}
	
	return num;
}

//Total number of attributes in the document
int getNumAttributes(const GPXdoc* doc){
	int i = 0;
	if(doc == NULL){
		return i;
	}
	TrackSegment *trkSeg = NULL;
	Track *trk = NULL;
	Waypoint *wpt = NULL;
	Route *rte = NULL;
	Node *node = NULL;
	Node *node2 = NULL;
	Node *node3 = NULL;
	Node *att = NULL;
	//If the document has Waypoints
	if(doc->waypoints){
		node = doc->waypoints->head;
		while(node){
			wpt = node->data;
			//If the waypoint has attributes
			if(wpt->attributes){
				att = wpt->attributes->head;
				while(att){
					i++;
					att = att->next;
				}
			}
			//If the waypoint has a name attribute
			if(strcmp(wpt->name, "")!= 0){
				i++;
			}
			node = node->next;
		}
	
	}
	
	
	//if the document has Routes
	if(doc->routes){
		node = doc->routes->head;
		while(node){
			rte = node->data;
			//If the route has attributes
			if(rte->attributes){
				att = rte->attributes->head;
				while(att){
					i++;
					att = att->next;
				}
			
			}
			//If the route has waypoints
			if(rte->waypoints){
				node2 = rte->waypoints->head;
				while(node2){
					wpt = node2->data;
					//If the waypoints have attributes
					if(wpt->attributes){
						att = wpt->attributes->head;
						while(att){
							i++;
							att = att->next;
						}
					}
					if(strcmp(wpt->name, "")!=0){
						i++;
					}
					node2 = node2->next;
				}
			}
			//If the route has a name attribute
			if(strcmp(rte->name, "")!=0){
				i++;
			}
		
			node = node->next;
		}
	}
	
	//If the document has tracks
	if(doc->tracks){
		node = doc->tracks->head;
		while(node){
			trk = node->data;
			//If the track has attributes
			if(trk->attributes){
				att = trk->attributes->head;
				while(att){
					i++;
					att = att->next;
				}
			}
			
			//if the track has track segments
			if(trk->segments){
				node2 = trk->segments->head;
				while(node2){
					trkSeg = node2->data;
					//if the segment has waypoints
					if(trkSeg->waypoints){
						node3 = trkSeg->waypoints->head;
						while(node3){
							wpt = node3->data;
							//If the waypoint has attributes
							if(wpt->attributes){
								att = wpt->attributes->head;
								while(att){
									i++;
									att = att->next;
								}
							}
							if(strcmp(wpt->name, "")!= 0){
								i++;
							}
							node3 = node3->next;
						}
					}
					node2 = node2->next;
				}
			}
			
			if(strcmp(trk->name, "")!= 0){
				i++;
			}
			node = node->next;
		}
	}
	
	return i;
}

// Function that returns a waypoint with the given name.  If more than one exists, return the first one.  
Waypoint* getWaypoint(const GPXdoc* doc, char* name){
	if(doc == NULL){
		return NULL;
	}
	List* wptL = doc->waypoints;
	Node* wptN = wptL->head;
	
	//Go through all of the Waypoint Nodes
	while(wptN){
		Waypoint* wpt = wptN->data;
		if(strcmp(name, wpt->name)==0){
			return wpt;
		}
		wptN = wptN->next;
	}
	return NULL;
}
// Function that returns a track with the given name.  If more than one exists, return the first one.  
Track* getTrack(const GPXdoc* doc, char* name){
	if(doc == NULL){
		return NULL;
	}
	List* trkL = doc->tracks;
	Node* trkN = trkL->head;
	
	//Go through all of the Track Nodes
	while(trkN){
		Track* trk = trkN->data;
		if(strcmp(name, trk->name)==0){
			return trk;
		}
		trkN = trkN->next;
	}
	return NULL;

}
// Function that returns a route with the given name.  If more than one exists, return the first one.  
Route* getRoute(const GPXdoc* doc, char* name){
	if(doc == NULL){
		return NULL;
	}
	List* rteL = doc->routes;
	Node* rteN = rteL->head;
	
	//Go through all of the Route Nodes
	while(rteN){
		Route* rte = rteN->data;
		if(strcmp(name, rte->name)==0){
			return rte;
		}
		rteN = rteN->next;
	}
	return NULL;

}//********** Assignment 2 Functions *********
//Module 1

bool validateGPXDoc(GPXdoc* doc, char* gpxSchemaFile){

	if(gpxSchemaFile == NULL){
		return FALSE;
	}
	if(doc == NULL){
		return FALSE;
	}
	xmlNode *node = NULL;
	xmlNsPtr ptr = NULL;
	xmlDoc *xDoc;
	xDoc = xmlNewDoc((xmlChar*) "1.0");
	
	//Create an xml tree from the GPXdoc
	vDocToXml(xDoc, doc);
	
	node = xDoc->children;
	
	ptr = xmlNewNs(NULL, (xmlChar*)doc->namespace, NULL);
	node->ns = ptr;
	
	//Validate
	xmlSchemaParserCtxtPtr ctxt;
	xmlSchemaPtr schema;
	
	xmlLineNumbersDefault(1);
	
	ctxt = xmlSchemaNewParserCtxt(gpxSchemaFile);
	
	xmlSchemaSetParserErrors(ctxt, (xmlSchemaValidityErrorFunc) err, (xmlSchemaValidityWarningFunc) err, stderr);
	schema = xmlSchemaParse(ctxt);
	xmlSchemaFreeParserCtxt(ctxt);
	

	
	xmlSchemaValidCtxtPtr ctxt2;
	int num = 0;
	
	ctxt2 = xmlSchemaNewValidCtxt(schema);
	//xmlSchemaSetValidErrors(ctxt2, (xmlSchemaValidityErrorFunc) err, (xmlSchemaValidityWarningFunc) err, stderr);
	num = xmlSchemaValidateDoc(ctxt2, xDoc);
	if(num == 0){
		xmlSchemaFreeValidCtxt(ctxt2);
		xmlFreeDoc(xDoc);

		if(schema != NULL){
			xmlSchemaFree(schema);
			xmlSchemaCleanupTypes();
			xmlCleanupParser();
			xmlMemoryDump();
		}
		return TRUE;
	}
	else{
		xmlSchemaFreeValidCtxt(ctxt2);
		xmlFreeDoc(xDoc);
		if(schema != NULL){
			xmlSchemaFree(schema);
			xmlSchemaCleanupTypes();
			xmlCleanupParser();
			xmlMemoryDump();
		}
		return FALSE;
	}
	
	
}


bool writeGPXdoc(GPXdoc* doc, char* fileName){
	
	if((doc == NULL)||(fileName == NULL)){
		return FALSE;
	}
		
	xmlDocPtr newDoc = NULL;
	xmlNode *node = NULL;
	xmlNsPtr ptr = NULL;

	//Create a new xmlDoc and the root node
	newDoc = xmlNewDoc(BAD_CAST "1.0");
	wDocToXml(newDoc, doc);
	
	node = newDoc->children;
	
	ptr = xmlNewNs(NULL, (xmlChar*)doc->namespace, NULL);
	node->ns = ptr;
	
	if(xmlSaveFormatFileEnc(fileName, newDoc, "UTF-8", 4) < 0){
		xmlFreeDoc(newDoc);
		xmlCleanupParser();
		return FALSE;
	}
	xmlFreeDoc(newDoc);
	xmlCleanupParser();
	
	return TRUE;
}


//Module 2

float getRouteLen(const Route *rt){
	
	//If there is no route
	if(rt == NULL){
		return 0;
	}

	float dist = 0;
	//Get the waypoints from the route
	Node* wptNode = rt->waypoints->head;
	//While there is a distance to calculate between this location and the next
	while(wptNode->next){
		Waypoint *wpt1 = wptNode->data;
		Waypoint *wpt2 = wptNode->next->data;
		//Calculate the distance between this and the next point and add it to the total distance
		dist = dist + distance(wpt1->latitude, wpt1->longitude, wpt2->latitude, wpt2->longitude);
		wptNode = wptNode->next;	
	}

	return dist;
}


float getTrackLen(const Track *tr){
	float dist = 0;
	Waypoint *wp1 = NULL;
	Waypoint *wp2 = NULL;
	TrackSegment *seg = NULL;
	TrackSegment *tempTrack = NULL;
	Node *wptNode = NULL;
	
	//If there is no track
	if(tr == NULL){
		return 0;
	}
	
	//Get the segments from the track
	Node *segNode = tr->segments->head;
	
	//Traverse through all the TrackSegments
	while(segNode){
		seg = segNode->data;
		//If there is two segments that need to be linked
		if(segNode->previous){
			//grab the previous Track segment
			tempTrack = segNode->previous->data;
			//grab the last waypoint from the previous track segment
			wp1 = tempTrack->waypoints->tail->data;
			//Get the first waypoint of the new track segment
			wptNode = seg->waypoints->head;
			wp2 = wptNode->data;
			//Add the distance between the two
			dist = dist + distance(wp1->latitude, wp1->longitude, wp2->latitude, wp2->longitude);

		}
		
		
		//Traverse through all the waypoints in each track segment
		wptNode = seg->waypoints->head;
		while(wptNode->next){
			wp1 = wptNode->data;
			wp2 = wptNode->next->data;
			dist = dist + distance(wp1->latitude, wp1->longitude, wp2->latitude, wp2->longitude);
			wptNode = wptNode->next;
		}
		segNode = segNode->next;
	}
	return dist;
}

float round10(float len){

	float floored = (float)floor(len);
	int a = (int)floored%10;
	
	if(a < 5){
		return floored - a;
	}
	else{
		return floored + (10-a);
	}

}


int numRoutesWithLength(const GPXdoc* doc, float len, float delta){
	int counter = 0;
	
	if(doc == NULL){
		return 0;
	}
	if(len < 0){
		return 0;
	}
	if(delta < 0){
		return 0;
	}
	
	//Get the first route
	Node *routes = doc->routes->head;
	
	while(routes){
		float a = getRouteLen(routes->data);
		//If the length of the current route is between given length +/- delta, add 1 to counter
		if((a > len-delta)&&(a < len+delta)){
			counter ++;
		}
		routes = routes->next;
	}

	return counter;
}



int numTracksWithLength(const GPXdoc* doc, float len, float delta){
	int counter = 0;
	
	if(doc == NULL){
		return 0;
	}
	if(len < 0){
		return 0;
	}
	if(delta < 0){
		return 0;
	}
	
	Node *tracks = doc->tracks->head;
	
	while(tracks){
		float a = getTrackLen(tracks->data);
		if((a > len - delta)&&(a < len + delta)){
			counter++;
		} 
		tracks = tracks->next;
	}
	
	return counter;
}


bool isLoopRoute(const Route* route, float delta){

	if(delta < 0){
		return FALSE;
	}
	if(route == NULL){
		return FALSE;
	}
	int counter = 0;
	
	Node* wpt1 = route->waypoints->head;
	Node* wpt2 = route->waypoints->tail;
	Waypoint *w1 = wpt1->data;
	Waypoint *w2 = wpt2->data;
	
	//If the first and last waypoint are farther away than the given delta, return false
	if(distance(w1->latitude, w1->longitude, w2->latitude, w2->longitude) > delta){
		return FALSE;
	}
	
	//Go through all of the route's waypoints
	while(wpt1){
		counter++;
		wpt1 = wpt1->next;
	}
	
	//If there is less than 4 waypoints
	if(counter < 4){
		return FALSE;
	}
	
	//If it passes all requirements, it is a loop
	return TRUE;
}


bool isLoopTrack(const Track *tr, float delta){
	if(tr == NULL){
		return FALSE;
	}
	if(delta < 0){
		return FALSE;
	}

	//make sure there are at least 4 points
	int counter = 0;

	//Get the first and last track segments
	Node *segments = tr->segments->head;
	Node *tailSeg = tr->segments->tail;
	//If there are no segments, return False
	if(segments == NULL){
		return FALSE;
	}
	TrackSegment *temp = segments->data;
	Node *wpNode1 = temp->waypoints->head;
	temp = tailSeg->data;
	Node *wpNode2 = temp->waypoints->tail;
	
	Waypoint *wpt1 = wpNode1->data;
	Waypoint *wpt2 = wpNode2->data;
	
	//If the distance between the first and last locations are farther away than delta
	if(distance(wpt1->latitude, wpt1->longitude, wpt2->latitude, wpt2->longitude) > delta){
		return FALSE;
	}
	
	while(segments){
		temp = segments->data;
		wpNode1 = temp->waypoints->head;
		while(wpNode1){
			counter++;
			wpNode1 = wpNode1->next;
		}
	
		segments = segments->next;
	}
	
	//If there is less than 4 waypoints it is False
	if(counter < 4){
		return FALSE;
	}

	//If it passes all the requirements, it is a loop
	return TRUE;
}


List* getRoutesBetween(const GPXdoc* doc, float sourceLat, float sourceLong, float destLat, float destLong, float delta){
	
	if(doc == NULL){
		return NULL;
	}
	if(delta < 0){
		return NULL;
	}
	if(doc->routes == NULL){
		return NULL;
	}
	//Create a list of Routes
	List* list = NULL;
	//Iter node to traverse through all of the routes
	Node *rIter = doc->routes->head;
	//Iter node to traverse through all of the waypoints
	Route *rte = NULL;
	Waypoint *wpt = NULL;
	
	//Looking through all routes
	while(rIter){
		rte = rIter->data;
		if(rte->waypoints){
			wpt = rte->waypoints->head->data;
			
			//If the first waypoint is in range
			if(distance(sourceLat, sourceLong, wpt->latitude, wpt->longitude)<delta){
				wpt = rte->waypoints->tail->data;
				//If the last waypoint is within range
				if(distance(destLat, destLong, wpt->latitude, wpt->longitude)<delta){
				//If there isn't anything in the list yet initialize it
				if(list == NULL){
					list = initializeList(routeToString, deleteRoute, compareRoutes);
				}
				//Add the route to the list
				insertBack(list, rte);
				}
			}
		}
	
		rIter = rIter->next;
	}

	return list;
}

List* getTracksBetween(const GPXdoc* doc, float sourceLat, float sourceLong, float destLat, float destLong, float delta){

	if(doc == NULL){
		return NULL;
	}
	if(delta < 0){
		return NULL;
	}
	if(doc->tracks == NULL){
		return NULL;
	}
	
	Track *trk = NULL;
	Waypoint *wpt = NULL;
	TrackSegment *seg = NULL;
	//Create a list
	List* list = NULL;
	
	Node *iter = doc->tracks->head;
	
	//Go through all of the tracks
	while(iter){
		trk = iter->data;
		
		if(trk->segments){
			//Get the first track segment
			seg = trk->segments->head->data;
			wpt = seg->waypoints->head->data;
			
			//If the first waypoint is within distance
			if(distance(sourceLat, sourceLong, wpt->latitude, wpt->longitude)<delta){
				//Get the last waypoint of the last segment
				seg = trk->segments->tail->data;
				wpt = seg->waypoints->tail->data;
				
				//If the last waypoint is within distance
				if(distance(destLat, destLong, wpt->latitude, wpt->longitude) < delta){
					if(list == NULL){
					//Create a track list
					list = initializeList(trackToString, deleteTrack, compareTracks);
					}
					insertBack(list, trk);
				
				}
			}
			
		}
	
		iter = iter->next;
	}
	
	return list;
}


//Module 3

//Use 15m tolerance for loop checking


char* trackToJSON(const Track *tr){

	if(tr == NULL){
		return "{}";
	}
	
	char* name = malloc(sizeof(char)+1);
	char* string = malloc(sizeof(char)+1);
	size_t size = 0;

	//Get the name of the Track
	if((strcmp(tr->name, "")==0)||(tr->name == NULL)){
		size = snprintf(NULL, 0, "None");
		name = realloc(name, size+1);
		sprintf(name, "None");
	}
	else{
		size = snprintf(NULL, 0, "%s", tr->name);
		name = realloc(name, size+1);
		sprintf(name, "%s", tr->name);
	}
	
	//Create a print statement based on Loop status
	if(!(isLoopTrack(tr, 15))){
		size = snprintf(NULL, 0, "{\"name\":\"%s\",\"len\":%.1f,\"loop\":false}", name, round10(getTrackLen(tr)));
		string = malloc(size+1);
		sprintf(string, "{\"name\":\"%s\",\"len\":%.1f,\"loop\":false}", name, round10(getTrackLen(tr)));
	}
	else{
		size = snprintf(NULL, 0, "{\"name\":\"%s\",\"len\":%.1f,\"loop\":true}", name, round10(getTrackLen(tr)));
		string = malloc(size+1);
		sprintf(string, "{\"name\":\"%s\",\"len\":%.1f,\"loop\":true}", name, round10(getTrackLen(tr)));
	}
	
	free(name);

	return string;
}


char* routeToJSON(const Route *rt){
	
	if(rt == NULL){
		return "{}";
	}

	char* name = malloc(sizeof(char)+1);
	char* string = malloc(sizeof(char)+1);
	size_t size = 0;

	//Get the name of the Track
	if((strcmp(rt->name, "")==0)||(rt->name == NULL)){
		size = snprintf(NULL, 0, "None");
		name = realloc(name, size+1);
		sprintf(name, "None");
	}
	else{
		size = snprintf(NULL, 0, "%s", rt->name);
		name = realloc(name, size+1);
		sprintf(name, "%s", rt->name);
	}
	
	//Get number of points along route
	int numVal = 0;
	
	Node* temp = rt->waypoints->head;
	
	while(temp){
		numVal++;
		temp = temp->next;
	}
	
	
	
	//Create a print statement based on Loop status
	if(!(isLoopRoute(rt, 15))){
		size = snprintf(NULL, 0, "{\"name\":\"%s\",\"numPoints\":%d,\"len\":%.1f,\"loop\":false}", name, numVal, round10(getRouteLen(rt)));
		string = malloc(size+1);
		sprintf(string, "{\"name\":\"%s\",\"numPoints\":%d,\"len\":%.1f,\"loop\":false}", name, numVal, round10(getRouteLen(rt)));
	}
	else{
		size = snprintf(NULL, 0, "{\"name\":\"%s\",\"numPoints\":%d,\"len\":%.1f,\"loop\":true}", name, numVal, round10(getRouteLen(rt)));
		string = malloc(size+1);
		sprintf(string, "{\"name\":\"%s\",\"numPoints\":%d,\"len\":%.1f,\"loop\":true}", name, numVal, round10(getRouteLen(rt)));
	}
	
	free(name);

	return string;
}



char* routeListToJSON(const List *list){

	if(list == NULL){
		return "[]";
	}

	Node* node = list->head;
	size_t size = snprintf(NULL, 0, "[");
	char* string = malloc(size+1);
	sprintf(string,"[");
	
	
	while(node){
		size = snprintf(NULL, 0, "%s%s", string, routeToJSON(node->data));
		string = realloc(string, size+1);
		strcat(string, routeToJSON(node->data));
		//If there is a next node add a comma
		if(node->next){
			size = snprintf(NULL, 0, "%s%c", string, ',');
			string = realloc(string, size+1);
			strcat(string, ",");
		}
		node = node->next;
	}
	
	size = snprintf(NULL, 0, "%s%s", string, "]");
	string = realloc(string, size+1);
	strcat(string, "]");
	
	
	return string;
}


char* trackListToJSON(const List *list){
	if(list == NULL){
		return "[]";
	}
	size_t size = snprintf(NULL, 0, "[");
	char* string = malloc(size+1);
	sprintf(string, "[");
	Node* temp = list->head;
	
	while(temp){
		size = snprintf(NULL, 0, "%s%s", string, trackToJSON(temp->data));
		string = realloc(string, size+1);
		strcat(string, trackToJSON(temp->data));
		
		//If there is another track to add
		if(temp->next){
			size = snprintf(NULL, 0, "%s%c", string, ',');
			string = realloc(string, size+1);
			strcat(string, ",");
		
		}
		temp = temp->next;
	}
	
	size = snprintf(NULL, 0, "%s%s", string, "]");
	string = realloc(string, size+1);
	strcat(string, "]");
	
	return string;
}



char* GPXtoJSON(const GPXdoc* gpx){
	if(gpx == NULL){
		return "{}";
	}

	size_t size = snprintf(NULL, 0, "{\"version\":%f,\"creator\":\"%s\",\"numWaypoints\":%d,\"numRoutes\":%d,\"numTracks\":%d}", gpx->version, gpx->creator, getNumWaypoints(gpx), getNumRoutes(gpx), getNumTracks(gpx));
	char* string = malloc(size+1);
	sprintf(string, "{\"version\":%.1f,\"creator\":\"%s\",\"numWaypoints\":%d,\"numRoutes\":%d,\"numTracks\":%d}", gpx->version, gpx->creator, getNumWaypoints(gpx), getNumRoutes(gpx), getNumTracks(gpx));
	
	return string;
}



// ***************************** Bonus A2 functions ********************************


void addWaypoint(Route *rt, Waypoint *pt){
	if(rt == NULL){
		return;
	}
	if(pt == NULL){
		return;
	}
	
	insertBack(rt->waypoints, pt);

}


void addRoute(GPXdoc* doc, Route* rt){
	if(doc == NULL){
		return;
	}
	if(rt == NULL){
		return;
	}
	
	insertBack(doc->routes, rt);

}


GPXdoc* JSONtoGPX(const char* gpxString){
	if(gpxString == NULL){
		return NULL;
	}
	//create the doc
	GPXdoc *doc = malloc(sizeof(GPXdoc));
	//add the namespace
	strcpy(doc->namespace, "http://www.topografix.com/GPX/1/1");
	
	//Initialize the lists
	doc->waypoints = initializeList(waypointToString, deleteWaypoint, compareWaypoints);
	doc->routes = initializeList(routeToString, deleteRoute, compareRoutes);
	doc->tracks = initializeList(trackToString, deleteTrack, compareTracks);
	

	return doc;

}


Waypoint* JSONtoWaypoint(const char* gpxString){
	if(gpxString == NULL){
		return NULL;
	}
	//Create an editable copy of the string
	char* temp = malloc(sizeof(gpxString)+1);
	strcpy(temp, gpxString);
	//Create the Waypoint
	Waypoint *wpt = malloc(sizeof(Waypoint));
	wpt->attributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

	return wpt;
}


Route* JSONtoRoute(const char* gpxString){
	if(gpxString == NULL){
		return NULL;
	}
	Route *rte = malloc(sizeof(Route));
	rte->waypoints = initializeList(waypointToString, deleteWaypoint, compareWaypoints);
	rte->attributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
	
	return rte;
}

char* parseGPX(char* filename){
	GPXdoc *doc = createGPXdoc(filename);
	if(doc == NULL){
		return "{}";
	}
	bool flag = validateGPXDoc(doc, "gpx.xsd");
	if(flag){
		char* text = GPXtoJSON(doc);
		deleteGPXdoc(doc);
		return text;
	}
	else{
		deleteGPXdoc(doc);
		return "{}";
	}

}

char* getRouteList(char* filename){
	GPXdoc *doc = createGPXdoc(filename);
	char* text = routeListToJSON(doc->routes);
	deleteGPXdoc(doc);
	return text;
}

char* getTrackList(char* filename){
	GPXdoc *doc = createGPXdoc(filename);
	char* text = trackListToJSON(doc->tracks);
	deleteGPXdoc(doc);
	return text;
}

char* attToJSON(const Attribute * att){
	for(int i = 0; i < strlen(att->value); i++){
		if(att->value[i] == '\n'){
			att->value[i] = '\0';
		}
	}
	size_t size = snprintf(NULL, 0, "{\"name\":\"%s\",\"value\":\"%s\"}", att->name, att->value);
	char* string = malloc(size+1);
	sprintf(string, "{\"name\":\"%s\",\"value\":\"%s\"}", att->name, att->value);
	return string;
}

char* attListToJSON(const List* list){
	if(list == NULL){
		return "[{}]";
	}
	size_t size = snprintf(NULL, 0, "[");
	char* string = malloc(size+1);
	sprintf(string, "[");
	Node* temp = list->head;
	
	while(temp){
		size = snprintf(NULL, 0, "%s%s", string, attToJSON(temp->data));
		string = realloc(string, size+1);
		strcat(string, attToJSON(temp->data));
		
		//If there is another track to add
		if(temp->next){
			size = snprintf(NULL, 0, "%s%c", string, ',');
			string = realloc(string, size+1);
			strcat(string, ",");
		
		}
		temp = temp->next;
	}
	
	size = snprintf(NULL, 0, "%s%s", string, "]");
	string = realloc(string, size+1);
	strcat(string, "]");
	return string;
}

char* rGetAttList(char* filename, int index){
	GPXdoc *doc = createGPXdoc(filename);
	Node *node = doc->routes->head;
	index = index-1;
	while(index > 0){
		node = node->next;
		index--;
	}
	Route *rte = node->data;
	char* text = attListToJSON(rte->attributes);
	deleteGPXdoc(doc);
	return text;
	
}

char* tGetAttList(char* filename, int index){
	GPXdoc *doc = createGPXdoc(filename);
	Node *node = doc->tracks->head;
	index = index-1;
	while(index > 0){
		node = node->next;
		index--;
	}
	Track *trk = node->data;
	char* text = attListToJSON(trk->attributes);
	deleteGPXdoc(doc);
	return text;
	
}

bool rRename(char* fileName, int n, char* newName){
	GPXdoc *doc = createGPXdoc(fileName);
	Node* node = doc->routes->head;
	n = n-1;
	//Find the right route to change the name of
	while(n > 0){
		node = node->next;
		n--;
	}
	Route *rte = node->data;
	free(rte->name);
	size_t size = snprintf(NULL, 0, "%s", newName);
	rte->name = malloc(size+1);
	strcpy(rte->name, newName);
	
	//Check to see if the new file would be valid
	bool flag1 = validateGPXDoc(doc, "gpx.xsd");
	
	//if new file is valid, write new file
	if(flag1){
		bool flag2 = writeGPXdoc(doc, fileName);
		deleteGPXdoc(doc);
		return flag2;
	}
	//If new file is not valid, return false
	else{
		return FALSE;
	}
}

bool tRename(char* fileName, int n, char* newName){
	GPXdoc *doc = createGPXdoc(fileName);
	Node *node = doc->tracks->head;
	n = n-1;
	
	//Find the right route to change the name of
	while(n > 0){
		node = node->next;
		n--;
	}
	Track *trk = node->data;
	free(trk->name);
	size_t size = snprintf(NULL, 0, "%s", newName);
	trk->name = malloc(size+1);
	strcpy(trk->name, newName);
	
	//Check if new file would be valid
	bool flag1 = validateGPXDoc(doc, "gpx.xsd");
	
	//If new file would be valid, create file
	if(flag1){
		bool flag2 = writeGPXdoc(doc, fileName);
		deleteGPXdoc(doc);
		return flag2;
	}
	else{
		deleteGPXdoc(doc);
		return FALSE;
	}
}



char* rPathList(char* fileName, float la1, float lo1, float la2, float lo2, float d){
	GPXdoc *doc = createGPXdoc(fileName);
	bool flag = validateGPXDoc(doc, "gpx.xsd");
	if(flag){
		List *list = getRoutesBetween(doc, la1, lo1, la2, lo2, d);
		deleteGPXdoc(doc);
		char* text = routeListToJSON(list);
		return text;
	}
	else{
		return "false";
	}

}

bool newRoute(char* fileName, char* routeName, float lat, float lon){
	GPXdoc *doc = createGPXdoc(fileName);
	Route *rte = malloc(sizeof(Route));
	Waypoint *wpt = malloc(sizeof(Waypoint));
	
	size_t size = snprintf(NULL, 0, "%s", routeName);
	rte->name = malloc(size+1);
	sprintf(rte->name, "%s", routeName);
	rte->waypoints = initializeList(waypointToString, deleteWaypoint, compareWaypoints);
	rte->attributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
	
	wpt->latitude = lat;
	wpt->longitude = lon;
	Node *node = initializeNode(wpt);
	rte->waypoints->head = node;
	
	addRoute(doc, rte);
	
	bool flag = validateGPXDoc(doc, "gpx.xsd");
	
	if(flag){
		bool flag2 = writeGPXdoc(doc, fileName);
		if(flag2){
			deleteGPXdoc(doc);
			return TRUE;
		}
		else{
			deleteGPXdoc(doc);
			deleteRoute(rte);
			return FALSE;
		}

	}
	else{
		deleteRoute(rte);
		deleteGPXdoc(doc);
		return FALSE;
	}

}