struct vec4{
  double x, double y, double z;
}

struct vec4{
  double x, double y, double z, double r;
}

struct point{
	double x, double y, double z;
};

class line {
   vec3 v;
	 vec3 offset;
   line(point p, point q){
       v.x = q.x-p.x;
       v.y = q.y-p.y;
       v.z = q.z-p.z;
       v.p = 0;
			 offset.x = p.x;
			 offset.y = p.y;
			 offset.z = p.z;
   };

   line(vec4 v1, vec4 v2) {
       if (v1.r == 0) {
			     v.x = v1.x;
			     v.y = v1.y;
			     v.z = v1.z;
			     offset.x = v2.x;
			     offset.y = v2.y;
			     offset.z = v2.z;
			 }
       if (v2.r == 0) {
			     v.x = v2.x;
			     v.y = v2.y;
			     v.z = v2.z;
			     offset.x = v1.x;
			     offset.y = v1.y;
			     offset.z = v1.z;
			 }
   };

	 vec4 homogeneousVec() {
			vec4 v;
			v.x = v.x;
			v.y = v.y;
			v.z = v.z;
			v.r = 0;
			return v;
   }

	 vec4 homogeneousPoint() {
			vec4 v;
			v.x = offset.x;
			v.y = offset.y;
			v.z = offset.z;
			v.r = 1;
			return v;
   }

	 
};

class plane {

};
