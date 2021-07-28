#include "ExtraHelp.h"
#include <math.h>

void parse(GPXdoc *parsed, xmlNode * root){
	//To traverse the xml tree
	xmlNode *cur_node = NULL;
	
	//temp node for accessing information in the xml tree
	xmlNode *temp_node = NULL;
	//The current property being examined
	xmlAttr *cur_prop = NULL;
	//The root property
	xmlAttr *prop = NULL;
	

	
	
	//Traversing through all of the nodes in the XML tree
	for(cur_node = root; cur_node; cur_node = cur_node->next){
		//Temp storage to hold/create/manipulate Waypoints
	Waypoint *wpt = NULL;
	
	//Temp storage to hold/create/manipulate Routes
	Route *rte = NULL;
	
	//Temp storage to hold/create/manipulate Attributes
	Attribute *att = NULL;
	
	//Temp storage to hold/create/manipulate Tracks
	Track *trk = NULL;
	
	//Temp storage to hold/create/manipulate Track Segments
	TrackSegment *trks = NULL;
	
	//To hold strings to be converted to floats/integers
	char* hold = NULL;
	
		if(cur_node->type == XML_ELEMENT_NODE){
			
		//*************************** Creating a Waypoint ***************************
			//If the node being parsed is a Waypoint
			if(strcmp("wpt", (char*)cur_node->name)==0){
				
				//If the waypoint is not in the correct spot
				if(strcmp("gpx", (char*)cur_node->parent->name)!=0){
					att = malloc(sizeof(Attribute));
					size_t size1 = snprintf(NULL, 0, "%s", (char*)cur_node->name);
					att->name = malloc(size1+1);
					att->value = malloc(sizeof("")+1);
					strcpy(att->name, (char*)cur_node->name);
					strcpy(att->value, "");
								
					//If its a route attribute
					if(strcmp((char*)cur_node->parent->name, "rte")==0){
						rte = parsed->routes->tail->data;
						insertBack(rte->attributes, att);
					}//close if route attribute
				
					//If its a track attribute
					else if(strcmp((char*)cur_node->parent->name, "trk")==0){
						
						trk = parsed->tracks->tail->data;
						insertBack(trk->attributes, att);
					}
					//if its a trkpt attribute
					else if(strcmp((char*)cur_node->parent->name, "trkpt")==0){
						trk = parsed->tracks->tail->data;
						trks = trk->segments->tail->data;
						wpt = trks->waypoints->tail->data;
						insertBack(wpt->attributes, att);
					}
				
					//If its a rtept attribute
					else if(strcmp((char*)cur_node->parent->name, "rtept")==0){
						rte = parsed->routes->tail->data;
						insertBack(rte->attributes, att);
					}
				
				
				}
				else{						
					//Initializing a new waypoint
					wpt = malloc(sizeof(Waypoint));
					wpt->longitude = 0;
					wpt->latitude = 0;
					wpt->name = (char*)malloc(sizeof(char)*2);
					strcpy(wpt->name, "");
					wpt->attributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
				
					//Filling out the created waypoint with the info generated
					prop = cur_node->properties;
				
					for(cur_prop = prop; cur_prop; cur_prop = cur_prop->next){
					
						if(strcmp((char*)cur_prop->name, "lat")==0){
							wpt->latitude = atof((char*)cur_prop->children->content);
						}
						if(strcmp((char*)cur_prop->name, "lon")==0){
							wpt->longitude = atof((char*)cur_prop->children->content);
						}
					}
				
					insertBack(parsed->waypoints, wpt);
				}
			}//Close if wpt
		//*************************** Done creating waypoint ***************************
			
			
		//*************************** Creating a Name ****************************
			//If the node being parsed is for a name
			else if(strcmp("name", (char*)cur_node->name)==0){

			
				//If the name is for a waypoint
				if(strcmp((char*)cur_node->parent->name, "wpt")==0){
					//If the waypoint is part of GPX
					if(strcmp((char*)cur_node->parent->parent->name, "gpx")==0){
						wpt = parsed->waypoints->tail->data;
						size_t size = snprintf(NULL, 0, "%s", (char*)cur_node->children->content);
						free(wpt->name);
						wpt->name = malloc(size+1);
						strcpy(wpt->name, (char*)cur_node->children->content);
					}
					//If the waypoint is part of anything else
					else{
						break;
					}
				}
				
				//If the name is for a route
				if(strcmp((char*)cur_node->parent->name, "rte")==0){
					//If route is correct
					if(strcmp((char*)cur_node->parent->parent->name, "gpx")==0){
						rte = parsed->routes->tail->data;
						size_t size = snprintf(NULL, 0, "%s", (char*)cur_node->children->content);
						free(rte->name);
						rte->name = malloc(size+1);
						strcpy(rte->name, (char*)cur_node->children->content);
					}
					//If the route is in the wrong spot
					else{
						break;
					}
				}
				
				//
				if(strcmp((char*)cur_node->parent->name, "rtept")==0){
					rte = parsed->routes->tail->data;
					wpt = rte->waypoints->tail->data;
					size_t size = snprintf(NULL, 0, "%s", (char*)cur_node->children->content);
					free(wpt->name);
					wpt->name = malloc(size+1);
					strcpy(wpt->name, (char*)cur_node->children->content);
				}
				
				//If the name is for a track
				if(strcmp((char*)cur_node->parent->name, "trk")==0){
					trk = parsed->tracks->tail->data;
					size_t size = snprintf(NULL, 0, "%s", (char*)cur_node->children->content);
					free(trk->name);
					trk->name = malloc(size+1);
					strcpy(trk->name, (char*)cur_node->children->content);
				}
				
				//If the name is for a trackpoint
				if(strcmp((char*)cur_node->parent->name, "trkpt")==0){
					trk = parsed->tracks->tail->data;
					trks = trk->segments->tail->data;
					wpt = trks->waypoints->tail->data;
					size_t size = snprintf(NULL, 0, "%s", (char*)cur_node->children->content);
					free(wpt->name);
					wpt->name = malloc(size+1);
					strcpy(wpt->name, (char*)cur_node->children->content);
				
				}
			}//close If name
		//*************************** Done creating a Name ***************************
			
			
		//*************************** Creating a Route ***************************
			//if the node being parsed is a Route
			else if(strcmp("rte", (char*)cur_node->name)==0){
				rte = malloc(sizeof(Route));
				rte->waypoints = initializeList(waypointToString, deleteWaypoint, compareWaypoints);
				rte->attributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
				rte->name = (char*)malloc(sizeof(char));
				strcpy(rte->name, "");
			
				insertBack(parsed->routes, rte);
			
			}//close If route
		//*************************** Done creating a Route ***************************
			
			
		//*************************** Creating a Track ***************************
			//If the node being parsed is a track
			else if(strcmp("trk", (char*)cur_node->name)==0){
				trk = (Track*)malloc(sizeof(Track));
				trk->name = (char*)malloc(sizeof(char));
				strcpy(trk->name, "");
				trk->attributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
				trk->segments = initializeList(trackSegmentToString, deleteTrackSegment, compareTrackSegments);
				
				insertBack(parsed->tracks, trk);
			}
		//***************************Done creating Track ***************************
			
			
		//*************************** Creating a Route Point ***************************
			else if(strcmp("rtept", (char*)cur_node->name)==0){
				wpt = (Waypoint*)malloc(sizeof(Waypoint));
				wpt->longitude = 0;
				wpt->latitude = 0;
				wpt->name = (char*)malloc(sizeof(char));
				strcpy(wpt->name, "");
				wpt->attributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
				
				//Filling out the created waypoint with the info generated
				prop = cur_node->properties;
				
				for(cur_prop = prop; cur_prop; cur_prop = cur_prop->next){
					
					if(strcmp((char*)cur_prop->name, "lat")==0){
						wpt->latitude = atof((char*)cur_prop->children->content);
					}
					if(strcmp((char*)cur_prop->name, "lon")==0){
						wpt->longitude = atof((char*)cur_prop->children->content);
					}
				}
				
				
				rte = parsed->routes->tail->data;
				insertBack(rte->waypoints, wpt);

			}
		//***************************Done creating Route Point ***************************
		
		
		//*************************** Creating a Track Segment ***************************
			else if(strcmp("trkseg", (char*)cur_node->name)==0){
				//Create a track segment
				trks = (TrackSegment*)malloc(sizeof(TrackSegment));
				trks->waypoints = initializeList(waypointToString, deleteWaypoint, compareWaypoints);
								
				//Put the track segment in a list
				trk = parsed->tracks->tail->data;
				insertBack(trk->segments, trks);
			}
		//*************************** Done creating Track Segment ***************************


		//*************************** Creating a Track Point ***************************
			else if(strcmp("trkpt", (char*)cur_node->name)==0){
				wpt = (Waypoint*)malloc(sizeof(Waypoint));
				wpt->longitude = 0;
				wpt->latitude = 0;
				wpt->name = (char*)malloc(sizeof(char));
				strcpy(wpt->name, "");
				wpt->attributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
				
				//Filling out the created waypoint with the info generated
				prop = cur_node->properties;
				
				for(cur_prop = prop; cur_prop; cur_prop = cur_prop->next){
					
					if(strcmp((char*)cur_prop->name, "lat")==0){
						wpt->latitude = atof((char*)cur_prop->children->content);
					}
					if(strcmp((char*)cur_prop->name, "lon")==0){
						wpt->longitude = atof((char*)cur_prop->children->content);
					}
				}
								
				trk = parsed->tracks->tail->data;
				trks = trk->segments->tail->data;
				
				insertBack(trks->waypoints, wpt);
				
			}
		//*************************** Done creating Track Point ***************************


		//*************************** Creating a GPX ***************************
			else if(strcmp("gpx", (char*)cur_node->name)==0){
				
				xmlNs *ptr = cur_node->ns;
				strcpy(parsed->namespace, (char*)ptr->href);
				free(ptr);
				prop = cur_node->properties;
				for(cur_prop = prop; cur_prop; cur_prop = cur_prop->next){
					temp_node = cur_prop->children;
					
					//Adding the version
					if(strcmp((char*)cur_prop->name, "version")==0){
						hold = realloc(hold, sizeof((char*)temp_node->content) +sizeof(char));
						strcpy(hold, (char*)temp_node->content);
						parsed->version =atof(hold);
					}
					//Adding the creator
					else if(strcmp((char*)cur_prop->name, "creator")==0){
						size_t size = snprintf(NULL, 0, "%s",(char*)temp_node->content); 
						parsed->creator = (char*)malloc(size+1);
						strcpy(parsed->creator, (char*)temp_node->content);
					}
				}//close property list
			}//close if GPX
		//*************************** Done creating GPX ***************************
		
		
		//*************************** Creating an attribute ***************************
			//If the node being parsed is a random attribute
			else{
				att = malloc(sizeof(Attribute));
				size_t size1 = snprintf(NULL, 0, "%s", (char*)cur_node->name);
				size_t size2 = snprintf(NULL, 0, "%s", (char*)cur_node->children->content);
				att->name = malloc(size1+1);
				att->value = malloc(size2+1);
				strcpy(att->name, (char*)cur_node->name);
				strcpy(att->value, (char*)cur_node->children->content);
				
				//If its an attribute for a waypoint
				if(strcmp((char*)cur_node->parent->name, "wpt")==0){
					wpt = parsed->waypoints->tail->data;
					insertBack(wpt->attributes, att);
				}//close if its a waypoint name
				
				//If its a route attribute
				else if(strcmp((char*)cur_node->parent->name, "rte")==0){
					rte = parsed->routes->tail->data;
					insertBack(rte->attributes, att);
				
				}//close if route attribute
				
				//If its a track attribute
				else if(strcmp((char*)cur_node->parent->name, "trk")==0){
					trk = parsed->tracks->tail->data;
					insertBack(trk->attributes, att);
				}
				//if its a trkpt attribute
				else if(strcmp((char*)cur_node->parent->name, "trkpt")==0){
					trk = parsed->tracks->tail->data;
					trks = trk->segments->tail->data;
					wpt = trks->waypoints->tail->data;
					insertBack(wpt->attributes, att);
				}
				
				//If its a rtept attribute
				else if(strcmp((char*)cur_node->parent->name, "rtept")==0){
					rte = parsed->routes->tail->data;
					insertBack(rte->attributes, att);
				}
			}//end else
		//*************************** Done creating Attribute ***************************

		}//close if XMLELEMENT
		parse(parsed, cur_node->children);
	}//close for loop traversing through nodes
}

//Calculates distance between two points in metres
float distance(float lat1, float lon1, float lat2, float lon2){
	float a = 0;
	float b = 0;
	float c = 0;
	//The earth's radius in m
	float earth = 6371000;
	
	//q will be for the latitude in radians
	float q1 = lat1* M_PI/180;
	float q2 = lat2 * M_PI/180;
	float dq = (lat2 - lat1) * M_PI/180;
	
	//l will be for the longitude in radians
	float dl = (lon2 - lon1) * M_PI/180;
	
	a = sin(dq/2) * sin(dq/2) + cos(q1) * cos(q2) * sin(dl/2) * sin(dl/2);
	b = 2 * atan2(sqrt(a), sqrt(1-a));
	c = b * earth;

	return c;
}


void wDocToXml(xmlDoc *newDoc, GPXdoc *doc){
	xmlNodePtr gpx = NULL;
	xmlNodePtr node = NULL;
	xmlNodePtr node1 = NULL;
	xmlNodePtr node2 = NULL;
	Node* traverse = NULL;
	Node* traverse2 = NULL;
	Node* traverse3 = NULL;
	Node* traverse4 = NULL;
	Waypoint* wpt = NULL;
	Route* rte = NULL;
	Track* trk = NULL;
	Attribute* att = NULL;
	TrackSegment* seg = NULL;
	char* numString = malloc(sizeof(char*)+1);
	size_t size = 0;
	
	gpx = xmlNewNode(NULL, BAD_CAST "gpx");
	//Assign the root node to the doc
	xmlDocSetRootElement(newDoc, gpx);
	
	//Add properties to GPX
	size = snprintf((char*)0, 0, "%.1f", doc->version)+1;
	free(numString);
	numString = malloc(sizeof(char*)*size + 1);
	snprintf(numString, size, "%.1f", doc->version);
	
	xmlNewProp(gpx, BAD_CAST "version", BAD_CAST numString);
	xmlNewProp(gpx, BAD_CAST "creator", BAD_CAST doc->creator);
	xmlNewNs(gpx, (xmlChar*)doc->namespace, NULL);


	//Add all of the waypoints as children of Gpx
	traverse = doc->waypoints->head;
	while(traverse){
		wpt = traverse->data;
		//Create a new wpt node
		node = xmlNewChild(gpx, NULL, BAD_CAST "wpt", NULL);
		
		//Get the latitude
		size = snprintf(NULL, 0, "%f", wpt->latitude);
		free(numString);
		numString = malloc(size+1);
		sprintf(numString, "%f", wpt->latitude);
		xmlNewProp(node, BAD_CAST "lat", BAD_CAST numString);
		
		//Get the longitude
		size = snprintf(NULL, 0, "%f", wpt->longitude);
		free(numString);
		numString = malloc(size+1);
		sprintf(numString, "%f", wpt->longitude);
		xmlNewProp(node, BAD_CAST "lon", BAD_CAST numString);
		
		if(strcmp(wpt->name, "")!= 0){
			xmlNewChild(node, NULL, BAD_CAST "name", BAD_CAST wpt->name);
		}
		
		traverse2 = wpt->attributes->head;
		while(traverse2){
			att = traverse2->data;
			xmlNewChild(node, NULL, BAD_CAST att->name, BAD_CAST att->value);
			traverse2 = traverse2->next;
		}
	
		traverse = traverse->next;
	}
	
	
	//Add all of the Routes as children of Gpx
	traverse = doc->routes->head;
	while(traverse){
		rte = traverse->data;
		node = xmlNewChild(gpx, NULL, BAD_CAST "rte", NULL);
		xmlNewChild(node, NULL, BAD_CAST "name", BAD_CAST rte->name);
		
		//Route Attributes
		traverse2 = rte->attributes->head;
		while(traverse2){
			att = traverse2->data;
			xmlNewChild(node, NULL, BAD_CAST att->name, BAD_CAST att->value);
			traverse2 = traverse2->next;
		}
		
		//Getting rtepts
		traverse2 = rte->waypoints->head;
		while(traverse2){
			node1 = xmlNewChild(node, NULL, BAD_CAST "rtept", NULL);
			wpt = traverse2->data;
			//Get the latitude
			size = snprintf(NULL, 0, "%f", wpt->latitude);
			free(numString);
			numString = malloc(size+1);
			sprintf(numString, "%f", wpt->latitude);
			xmlNewProp(node1, BAD_CAST "lat", BAD_CAST numString);
		
			//Get the longitude
			size = snprintf(NULL, 0, "%f", wpt->longitude);
			free(numString);
			numString = malloc(size+1);
			sprintf(numString, "%f", wpt->longitude);
			xmlNewProp(node1, BAD_CAST "lon", BAD_CAST numString);
		
			if(strcmp(wpt->name, "")!= 0){
				xmlNewChild(node1, NULL, BAD_CAST "name", BAD_CAST wpt->name);
			}
			
			//Getting all of the rtept's attributes
			traverse3 = wpt->attributes->head;
			while(traverse3){
				att = traverse3->data;
				xmlNewChild(node1, NULL, BAD_CAST att->name, BAD_CAST att->value);
				traverse3 = traverse3->next;
			}

			traverse2 = traverse2->next;
		}
	
		traverse = traverse->next;
	}
	
	
	//Add all of the Tracks as children of GPX
	traverse = doc->tracks->head;
	while(traverse){
		trk = traverse->data;
		//Create a new track
		node = xmlNewChild(gpx, NULL, BAD_CAST "trk", NULL);
		
		//Get the name of the track
		xmlNewChild(node, NULL, BAD_CAST "name", BAD_CAST trk->name);
		
		//Get the current tracks attributes
		traverse2 = trk->attributes->head;
		while(traverse2){
			att = traverse2->data;
			xmlNewChild(node, NULL, BAD_CAST att->name, BAD_CAST att->value);
			traverse2 = traverse2->next;
		}
		
		
		//Get the Track Segments
		traverse2 = trk->segments->head;
		while(traverse2){
			//Get the current segment and add it as a child
			seg = traverse2->data;
			node1 = xmlNewChild(node, NULL, BAD_CAST "trkseg", NULL);
			
			//Get the current segment's waypoints
			traverse3 = seg->waypoints->head;
			while(traverse3){
				//Get the current waypoint's data and add it as a child
				wpt = traverse3->data;
				node2 = xmlNewChild(node1, NULL, BAD_CAST "trkpt", NULL);
				
				//Get the latitude
				size = snprintf(NULL, 0, "%f", wpt->latitude);
				free(numString);
				numString = malloc(size+1);
				sprintf(numString, "%f", wpt->latitude);
				xmlNewProp(node2, BAD_CAST "lat", BAD_CAST numString);
		
				//Get the longitude
				size = snprintf(NULL, 0, "%f", wpt->longitude);
				free(numString);
				numString = malloc(size+1);
				sprintf(numString, "%f", wpt->longitude);
				xmlNewProp(node2, BAD_CAST "lon", BAD_CAST numString);
			
				if(strcmp(wpt->name, "")!= 0){
					xmlNewChild(node2, NULL, BAD_CAST "name", BAD_CAST wpt->name);
				}
				
				//Get all of the trackpoint's attributes
				traverse4 = wpt->attributes->head;
				while(traverse4){
					att = traverse4->data;
					xmlNewChild(node2, NULL, BAD_CAST att->name, BAD_CAST att->value);
					
					traverse4 = traverse4->next;
				}
			
				traverse3 = traverse3->next;
			}
			
			traverse2 = traverse2->next;
		}
	
		traverse = traverse->next;
	}
	free(numString);
	xmlCleanupParser();
}


void vDocToXml(xmlDoc *newDoc, GPXdoc *doc){
	xmlNodePtr gpx = NULL;
	xmlNodePtr node = NULL;
	xmlNodePtr node1 = NULL;
	xmlNodePtr node2 = NULL;
	Node* traverse = NULL;
	Node* traverse2 = NULL;
	Node* traverse3 = NULL;
	Node* traverse4 = NULL;
	Waypoint* wpt = NULL;
	Route* rte = NULL;
	Track* trk = NULL;
	Attribute* att = NULL;
	TrackSegment* seg = NULL;
	xmlNsPtr ptr = xmlNewNs(NULL, (xmlChar*)doc->namespace,NULL);
	char* numString = malloc(sizeof(char*)+1);
	size_t size = 0;
	
	gpx = xmlNewNode(NULL, BAD_CAST "gpx");
	//Assign the root node to the doc
	xmlDocSetRootElement(newDoc, gpx);
	gpx->ns = ptr;
	
	//Add properties to GPX
	size = snprintf((char*)0, 0, "%.1f", doc->version)+1;
	free(numString);
	numString = malloc(sizeof(char*)*size + 1);
	snprintf(numString, size, "%.1f", doc->version);
	
	xmlNewProp(gpx, BAD_CAST "version", BAD_CAST numString);
	xmlNewProp(gpx, BAD_CAST "creator", BAD_CAST doc->creator);

	//Add all of the waypoints as children of Gpx
	traverse = doc->waypoints->head;
	while(traverse){
		wpt = traverse->data;
		//Create a new wpt node
		node = xmlNewChild(gpx, NULL, BAD_CAST "wpt", NULL);
		
		//Get the latitude
		size = snprintf(NULL, 0, "%f", wpt->latitude);
		free(numString);
		numString = malloc(size+1);
		sprintf(numString, "%f", wpt->latitude);
		xmlNewProp(node, BAD_CAST "lat", BAD_CAST numString);
		
		//Get the longitude
		size = snprintf(NULL, 0, "%f", wpt->longitude);
		free(numString);
		numString = malloc(size+1);
		sprintf(numString, "%f", wpt->longitude);
		xmlNewProp(node, BAD_CAST "lon", BAD_CAST numString);
		
		if(strcmp(wpt->name, "")!= 0){
			xmlNewChild(node, NULL, BAD_CAST "name", BAD_CAST wpt->name);
		}
		
		traverse2 = wpt->attributes->head;
		while(traverse2){
			att = traverse2->data;
			xmlNewChild(node, NULL, BAD_CAST att->name, BAD_CAST att->value);
			traverse2 = traverse2->next;
		}
	
		traverse = traverse->next;
	}
	
	
	//Add all of the Routes as children of Gpx
	traverse = doc->routes->head;
	while(traverse){
		rte = traverse->data;
		node = xmlNewChild(gpx, NULL, BAD_CAST "rte", NULL);
		xmlNewChild(node, NULL, BAD_CAST "name", BAD_CAST rte->name);
		
		//Route Attributes
		traverse2 = rte->attributes->head;
		while(traverse2){
			att = traverse2->data;
			xmlNewChild(node, NULL, BAD_CAST att->name, BAD_CAST att->value);
			traverse2 = traverse2->next;
		}
		
		//Getting rtepts
		traverse2 = rte->waypoints->head;
		while(traverse2){
			node1 = xmlNewChild(node, NULL, BAD_CAST "rtept", NULL);
			wpt = traverse2->data;
			//Get the latitude
			size = snprintf(NULL, 0, "%f", wpt->latitude);
			free(numString);
			numString = malloc(size+1);
			sprintf(numString, "%f", wpt->latitude);
			xmlNewProp(node1, BAD_CAST "lat", BAD_CAST numString);
		
			//Get the longitude
			size = snprintf(NULL, 0, "%f", wpt->longitude);
			free(numString);
			numString = malloc(size+1);
			sprintf(numString, "%f", wpt->longitude);
			xmlNewProp(node1, BAD_CAST "lon", BAD_CAST numString);
		
			if(strcmp(wpt->name, "")!= 0){
				xmlNewChild(node1, NULL, BAD_CAST "name", BAD_CAST wpt->name);
			}
			
			//Getting all of the rtept's attributes
			traverse3 = wpt->attributes->head;
			while(traverse3){
				att = traverse3->data;
				xmlNewChild(node1, NULL, BAD_CAST att->name, BAD_CAST att->value);
				traverse3 = traverse3->next;
			}

			traverse2 = traverse2->next;
		}
	
		traverse = traverse->next;
	}
	
	
	//Add all of the Tracks as children of GPX
	traverse = doc->tracks->head;
	while(traverse){
		trk = traverse->data;
		//Create a new track
		node = xmlNewChild(gpx, NULL, BAD_CAST "trk", NULL);
		
		//Get the name of the track
		xmlNewChild(node, NULL, BAD_CAST "name", BAD_CAST trk->name);
		
		//Get the current tracks attributes
		traverse2 = trk->attributes->head;
		while(traverse2){
			att = traverse2->data;
			xmlNewChild(node, NULL, BAD_CAST att->name, BAD_CAST att->value);
			traverse2 = traverse2->next;
		}
		
		
		//Get the Track Segments
		traverse2 = trk->segments->head;
		while(traverse2){
			//Get the current segment and add it as a child
			seg = traverse2->data;
			node1 = xmlNewChild(node, NULL, BAD_CAST "trkseg", NULL);
			
			//Get the current segment's waypoints
			traverse3 = seg->waypoints->head;
			while(traverse3){
				//Get the current waypoint's data and add it as a child
				wpt = traverse3->data;
				node2 = xmlNewChild(node1, NULL, BAD_CAST "trkpt", NULL);
				
				//Get the latitude
				size = snprintf(NULL, 0, "%f", wpt->latitude);
				free(numString);
				numString = malloc(size+1);
				sprintf(numString, "%f", wpt->latitude);
				xmlNewProp(node2, BAD_CAST "lat", BAD_CAST numString);
		
				//Get the longitude
				size = snprintf(NULL, 0, "%f", wpt->longitude);
				free(numString);
				numString = malloc(size+1);
				sprintf(numString, "%f", wpt->longitude);
				xmlNewProp(node2, BAD_CAST "lon", BAD_CAST numString);
			
				if(strcmp(wpt->name, "")!= 0){
					xmlNewChild(node2, NULL, BAD_CAST "name", BAD_CAST wpt->name);
				}
				
				//Get all of the trackpoint's attributes
				traverse4 = wpt->attributes->head;
				while(traverse4){
					att = traverse4->data;
					xmlNewChild(node2, NULL, BAD_CAST att->name, BAD_CAST att->value);
					
					traverse4 = traverse4->next;
				}
			
				traverse3 = traverse3->next;
			}
			
			traverse2 = traverse2->next;
		}
	
		traverse = traverse->next;
	}
	free(numString);
	xmlCleanupParser();
}

bool err(){

	return FALSE;
}

