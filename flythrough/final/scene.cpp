#include "scene.h"
using namespace std;

string Scene :: nexttoken(void) {
  char c;
  string token;
  int lastchar = 1;
  if (!f_in) {return(token); }
  if (f_in->eof()) {return(token);}
  while (f_in->get(c)) 
    {
      if (f_in->eof()) {
	return(token);
      }
      switch (c) {
      case '\n': nextline += 1;
      case ' ' :
      case '\t':
      case '\a':
      case '\b':
      case '\v':
      case '\f':
      case '\r': {
	if ( lastchar == 0 ) {return(token);}break; }
      case '{': {
	token = c; return(token); break;}
      case '}': {
	token = c;
	return(token);
	break; }
      case '!': {
	while ( c != '\n' && f_in->get(c)) {
	}
	nextline++; break;}
      default: {
	token = token + c;
	lastchar = 0;
	
	if ((f_in->peek() == '{') ||
	    (f_in->peek() == '}') ) {
	  if ( lastchar == 0 ) {
	    return(token);
	  } else {
	    f_in->get(c);
	    token = c;
	    return(token);
	  }
	}
	line = nextline;
      }
      }
    }
  return(" ");
}

float Scene :: getFloat() {
  strstream tmp;
  float number;
  string str = nexttoken();  
  tmp << str;
  if(!(tmp >> number))
    {
      cerr << "Line " << line << ": error getting float" << endl;
      exit(-1);
    }
  else 
    {
      char t;
      if ( (tmp >> t ) ) 
	{
	  cerr << "Line " << line << ": bum chars in number" << endl;
	  exit(-1);    
	} 
    }
  return number;
}

bool Scene :: isidentifier(string keyword) {
  string temp = keyword;
  if (!isalpha(temp[0])) return(false);
  for (int count = 1; count < temp.length(); count++) {
    if ((!isalnum(temp[count]))&& (temp[count]!='.')) return(false);
  }
  return(true);
}

void Scene :: cleanUp() {
  affStk.releaseAffines();
  def_stack->release();
  delete def_stack;
}

void Scene :: freeScene()  {
  GeomObj *p = obj;
  while(p)
    {
      GeomObj* q = p;
      p = p->next;
      delete q;
    }
  Light * q = light;
  while(q)
    {
      Light* r = q;
      q = q->next;
      delete r;
    }
}

mTokenType Scene :: whichtoken(string keyword) {
  string temp = keyword;
  if ( temp == "light" )	    return LIGHT;
  if ( temp == "rotate" )           return ROTATE;
  if ( temp == "translate" )        return TRANSLATE;
  if ( temp == "scale")             return (SCALE);	
  if ( temp == "push")        	    return (PUSH);
  if ( temp == "pop")		    return (POP);
  if ( temp == "identityAffine")    return (IDENTITYAFFINE);
  if ( temp == "cube")              return (CUBE);
  if ( temp == "sphere")            return (SPHERE);
  if ( temp == "torus")             return (TORUS);
  if ( temp == "plane")             return (PLANE);
  if ( temp == "square")            return (SQUARE);
  if ( temp == "triangle")					return (TRIANGLE);
  if ( temp == "cylinder")          return (CYLINDER);
  if ( temp == "taperedCylinder")   return (TAPEREDCYLINDER);
  if ( temp == "cone")              return (CONE);
  if ( temp == "tetrahedron")       return (TETRAHEDRON);
  if ( temp == "octahedron")        return (OCTAHEDRON);
  if ( temp == "dodecahedron")      return (DODECAHEDRON);
  if ( temp == "icosahedron")       return (ICOSAHEDRON);
  if ( temp == "buckyball")         return (BUCKYBALL);
  if ( temp == "wineglass")         return (WINEGLASS);
  if ( temp == "diamond")           return (DIAMOND);
  if ( temp == "teapot")	    return (TEAPOT);
  if ( temp == "union")             return (UNION);
  if ( temp == "intersection")      return (INTERSECTION);
  if ( temp == "difference")        return (DIFFERENCEa);
  if ( temp == "mesh")              return (MESH); 
  if ( temp == "makePixmap")        return (MAKEPIXMAP);
  if ( temp == "defaultMaterials")  return (DEFAULTMATERIALS);
  if ( temp == "ambient")           return (AMBIENT);
  if ( temp == "diffuse")           return (DIFFUSE);
  if ( temp == "specular")          return (SPECULAR);
  if ( temp == "specularFraction")  return (SPECULARFRACTION);
  if ( temp == "surfaceRoughness")  return (SURFACEROUGHNESS);
  if ( temp == "emissive")          return (EMISSIVE);
  if ( temp == "specularExponent")  return (SPECULAREXPONENT);
  if ( temp == "speedOfLight")      return (SPEEDOFLIGHT);
  if ( temp == "transparency")      return (TRANSPARENCY);
  if ( temp == "reflectivity")      return (REFLECTIVITY);
  if ( temp == "parameters")        return (PARAMETERS);
  if ( temp == "texture")	    return (TEXTURE);
  if ( temp == "globalAmbient")	    return (GLOBALAMBIENT);
  if ( temp == "minReflectivity")   return (MINREFLECTIVITY);
  if ( temp == "minTransparency")   return (MINTRANSPARENCY);
  if ( temp == "maxRecursionDepth") return (MAXRECURSIONDEPTH);
  if ( temp == "background")        return (BACKGROUND);
  if ( temp == "{")                 return (LFTCURLY);
  if ( temp == "}")                 return (RGHTCURLY);
  if ( temp == "def")               return (DEF);
  if ( temp == "use")               return (USE);
  if ( temp == " " )                return (T_NULL);
  if ( isidentifier(temp) )         return (IDENT);
  cout << temp << ":" << temp.length() << endl;
  return(UNKNOWN);
}

void Scene :: drawSceneOpenGL() {
  for(GeomObj* p = obj; p ; p = p->next)
    p->drawOpenGL();
}

bool Scene:: read(string fname) {
  file_in = new ifstream(fname.c_str());
  if(!(*file_in))
    { 
      cout << "I can't find or open file: " << fname << endl; 
      return false;
    }
  f_in = new strstream();
  line = nextline = 1;
  def_stack = new DefUnitStack();
  char ch;
  freeScene();
  obj = tail = NULL;
  light = NULL;
  affStk.tos = new AffineNode;
  affStk.tos->next = NULL;
  
  while (file_in->get(ch)) {*f_in << ch;}
  while(1)
    {
      GeomObj * shp = getObject();
      if(!shp) break;
      shp->next = NULL;
      if(obj == NULL){ obj = tail = shp;}
      else{tail->next = shp; tail = shp;}
    }	
  file_in->close();
  cleanUp();
  return true;
}

void Scene:: makeLightsOpenGL() {
  int num, lightNum = GL_LIGHT0; 
  Light *p; 
  float colr[4], posn[4];
  for(p = light, num = 0; p && num < 8; p = p-> next, lightNum++, num++) { 
    glEnable(lightNum); 
    (p->color).build4tuple(colr); 
    (p->pos).build4tuple(posn); 
    glLightfv(lightNum,GL_DIFFUSE,colr); 
    glLightfv(lightNum,GL_SPECULAR,colr); 
    glLightfv(lightNum,GL_POSITION,posn);
  }
}


GeomObj* Scene :: getObject() {
  string s;			
  GeomObj * newShape;
  mTokenType typ;
  while ((typ = (whichtoken( s = nexttoken() ))) != T_NULL) 
    {
      if(typ == UNION || typ == INTERSECTION || typ == DIFFERENCEa)
	{
	  switch(typ)
	    {
	    case UNION:				newShape = new UnionBool();	break;
	    case INTERSECTION:	newShape = new IntersectionBool();	break;
	    case DIFFERENCEa:		newShape = new DifferenceBool();break;
	    } // end of little switch
	  GeomObj* p = newShape;
	  p = getObject();
	  if(!p) return NULL;
	  ((Boolean*)newShape)->left  = p;
	  p = getObject();
	  if(!p) return NULL;
	  ((Boolean*)newShape)->right = p;
	  return newShape;
	}
      switch(typ) 
	{
	case LIGHT: {
	  Point3 p;
	  Color3 c;
	  p.x = getFloat(); p.y = getFloat();	p.z = getFloat();
	  c.red = getFloat(); c.green = getFloat();	c.blue = getFloat();
	  Light *l = new Light;
	  l->setPosition(p);
	  l->setColor(c);
	  l->next = light; //put it on the list
	  light = l; break;}
	case ROTATE: {
	  float angle;
	  Vector3 u;
	  angle = getFloat(); u.x = getFloat(); 
	  u.y = getFloat(); u.z = getFloat();
	  affStk.rotate(angle,u);break;}
	case TRANSLATE: {
	  Vector3 d;
	  d.x = getFloat(); d.y = getFloat(); d.z = getFloat();
	  affStk.translate(d);break;}
	case SCALE: {
	  float sx, sy, sz;
	  sx = getFloat(); sy = getFloat(); sz = getFloat();
	  affStk.scale(sx, sy, sz);break;}
	case PUSH: affStk.dup(); break; 
	case POP:  affStk.popAndDrop(); break;
	case IDENTITYAFFINE: affStk.setIdentity();break;		
	case AMBIENT: {
	  float dr, dg, db;
	  dr = getFloat(); dg = getFloat(); db = getFloat();
	  currMtrl.ambient.set(dr,dg,db); break;}
	case DIFFUSE: {
	  float dr,dg,db;
	  dr = getFloat(); dg = getFloat(); db = getFloat();
	  currMtrl.diffuse.set(dr,dg,db); break;}
	case SPECULAR:{ 
	  float dr,dg,db;
	  dr = getFloat(); dg = getFloat(); db = getFloat();
	  currMtrl.specular.set(dr,dg,db); break;}
	case EMISSIVE: {
	  float dr,dg,db;
	  dr = getFloat(); dg = getFloat(); db = getFloat();
	  currMtrl.emissive.set(dr,dg,db); break;}
	case PARAMETERS: {
	  currMtrl.numParams = (int)getFloat();
	  for(int i = 0; i < currMtrl.numParams; i++)
	    currMtrl.params[i] = getFloat();
	  break;}			
	case SPECULARFRACTION: currMtrl.specularFraction = getFloat(); break;
	case SURFACEROUGHNESS: currMtrl.surfaceRoughness = getFloat(); break;
	case TEXTURE: {
	  currMtrl.textureType = int(getFloat());}	
	  break;
	case DEFAULTMATERIALS: 	currMtrl.setDefault();break;
	case SPEEDOFLIGHT: currMtrl.speedOfLight = getFloat(); break;
	case SPECULAREXPONENT: currMtrl.specularExponent = getFloat(); break;
	case TRANSPARENCY:currMtrl.transparency = getFloat(); break;
	case REFLECTIVITY: currMtrl.reflectivity = getFloat(); break;
	case GLOBALAMBIENT:
	  ambient.red = getFloat(); ambient.green = getFloat();
	  ambient.blue = getFloat(); break;
	case BACKGROUND: 
	  background.red = getFloat();
	  background.green = getFloat(); 
	  background.blue = getFloat();break;
	case MINREFLECTIVITY: minReflectivity = getFloat(); break;
	case MINTRANSPARENCY:minTransparency = getFloat(); break;
	case MAXRECURSIONDEPTH: maxRecursionDepth = int(getFloat()); break;
	case MAKEPIXMAP: {
	  break;}
	case T_NULL: break;
	case DEF: {
	  string name, temp, lb, rb;
	  int l = line;
	  string inp;
	  name = nexttoken();
	  if ( whichtoken(name) != IDENT ) {
	    cout << "Error:  Identifier expected." << endl;
	    return NULL;
	  }
	  if ( def_stack->search(name) ) {
	    cout << line << ": " << name;
	    cout << ": attempt to redefine. " << endl;
	    return NULL;
	  }
	  lb = nexttoken();
	  if ( whichtoken(lb) != LFTCURLY ) {
	    cout << "Error: { expected." << endl;
	    return NULL;
	  }
	  while ( whichtoken( temp = nexttoken()) != RGHTCURLY ) {
	    cout << temp << endl;
	    inp = inp + temp + " ";
	    if (!f_in) {
	      cout << "Error: end of file detected." << endl;
	      return NULL;
	    }
	  }
	  def_stack->push(name, inp);
	  break;}
	case USE: {
	  string name;
	  name = nexttoken();
	  if ( whichtoken(name) != IDENT ) {
	    cout << line << ": " << name;
	    cout << ": identifier expected.";
	    return NULL;
	  }
	  if (! def_stack->search(name) ) {
	    cout << line << ": " << name;
	    cout << ": not defined.";
	    return NULL;
	  }
	  cout << def_stack->contents(name) << endl;
	  strstream *temp_fin = new strstream;
	  *temp_fin << def_stack->contents(name) << " ";
	  *temp_fin << f_in->rdbuf();
	  delete (f_in);
	  f_in = temp_fin;
	  break; }
	default:  {
	  switch(typ) 
	    {
	    case CUBE:		newShape = new Cube;break;
	    case SPHERE:       	newShape = new Sphere;break;
	    case TORUS:		newShape = new Torus;break;
	    case PLANE:		newShape = new Plane;break;
	    case SQUARE:		newShape = new Square;break;
	    case TRIANGLE:		newShape = new Triangle;break;
	    case TAPEREDCYLINDER:	newShape = new TaperedCylinder; 
	      ((TaperedCylinder*)newShape)->smallRadius = getFloat(); break;
	    case CONE:			newShape = new Cone;break; 
	    case CYLINDER:		newShape = new TaperedCylinder; 
	      ((TaperedCylinder*)newShape)->smallRadius = 1; break;
	      
	    case TETRAHEDRON:	newShape = new Tetrahedron;break;
	    case OCTAHEDRON:	newShape = new Octahedron;break;
	    case DODECAHEDRON:	newShape = new Dodecahedron; break;
	    case ICOSAHEDRON:	newShape = new Icosahedron; break;
	    case TEAPOT:       	newShape = new Teapot; break;

	    case MESH: 
	      {
		string fname = nexttoken();
		newShape = new Mesh(fname); 
		break;
	      }
	      
	    default: {
	      cerr << "Line " << nextline << ": unknown keyword " << s << endl;
	      return NULL;
	    }	
	    }
	  ((Shape*)newShape)->mtrl.set(currMtrl);
	  ((Shape*)newShape)->transf.set(*(affStk.tos->affn));
	  ((Shape*)newShape)->invTransf.set(*(affStk.tos->invAffn));
	  return newShape;
	}
	}
    }
  return NULL;
}
