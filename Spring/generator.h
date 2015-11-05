//
//  generator.h
//  Spring
//
//  Created by Matthew Dillard on 11/2/15.
//  
//

#ifndef generator_h
#define generator_h

namespace gen {
    state::vector test() {
        state::vector rvalue;
        rvalue.add_vertex({{10,10,10},{0,0,0},1.0,false});
        rvalue.add_vertex({{20,10,10},{0,0,0},1.0,true});
        
        state::edges.push_back({0.5,0.1,5.0,0,1});
        return rvalue;
    }
};

#endif /* generator_h */
