#pragma once 

#include "boost/range/algorithm/set_algorithm.hpp"

#include <crab/config.h>
#include <crab/common/debug.hpp>
#include <crab/common/stats.hpp>
#include <crab/common/types.hpp>
#include <crab/domains/operators_api.hpp>
#include <crab/domains/domain_traits.hpp>
#include <crab/domains/intervals.hpp>

namespace crab {
   namespace domains {
      typedef enum { APRON_INT, 
                     APRON_OCT, 
                     APRON_OPT_OCT, 
                     APRON_PK } apron_domain_id_t;
   }
}

#ifndef HAVE_APRON
/*
 * Dummy implementation if Apron not found 
 */
#define APRON_NOT_FOUND "No Apron. Run cmake with -DUSE_APRON=ON"

namespace crab {
   namespace domains {
      template<typename Number, typename VariableName, apron_domain_id_t ApronDom>
      class apron_domain: 
	public abstract_domain<Number,VariableName,apron_domain<Number,VariableName,ApronDom> > {
              
       public:
        typedef apron_domain<Number, VariableName, ApronDom> apron_domain_t;
	typedef abstract_domain<Number, VariableName, apron_domain_t> abstract_domain_t;
        using typename abstract_domain_t::linear_expression_t;
        using typename abstract_domain_t::linear_constraint_t;
        using typename abstract_domain_t::linear_constraint_system_t;
        using typename abstract_domain_t::variable_t;
        using typename abstract_domain_t::number_t;
        using typename abstract_domain_t::varname_t;
        typedef interval <Number> interval_t;

        apron_domain() {}    

        static apron_domain_t top() { CRAB_ERROR (APRON_NOT_FOUND); }

        static apron_domain_t bottom() { CRAB_ERROR (APRON_NOT_FOUND); }

        apron_domain (const apron_domain_t& other) {}
        
        bool is_bottom() { CRAB_ERROR (APRON_NOT_FOUND); }

        bool is_top() { CRAB_ERROR (APRON_NOT_FOUND); }

        bool operator<=(apron_domain_t other) { CRAB_ERROR (APRON_NOT_FOUND); }

        void operator|=(apron_domain_t other)
        { CRAB_ERROR (APRON_NOT_FOUND); }
        
        apron_domain_t operator|(apron_domain_t other)
        { CRAB_ERROR (APRON_NOT_FOUND); }
        
        apron_domain_t operator&(apron_domain_t other) 
        { CRAB_ERROR (APRON_NOT_FOUND); }
        
        apron_domain_t operator||(apron_domain_t other)
        { CRAB_ERROR (APRON_NOT_FOUND); }
        
        template<typename Thresholds>
        apron_domain_t widening_thresholds (apron_domain_t e, const Thresholds &ts) 
        { CRAB_ERROR (APRON_NOT_FOUND); }

        apron_domain_t operator&& (apron_domain_t other) 
        { CRAB_ERROR (APRON_NOT_FOUND); }
        
        void operator-=(variable_t var) 
        { CRAB_ERROR (APRON_NOT_FOUND); }

        interval_t operator[](variable_t v) 
        { CRAB_ERROR (APRON_NOT_FOUND); }

        void set(variable_t v, interval_t ival) 
        { CRAB_ERROR (APRON_NOT_FOUND); }

        void operator += (linear_constraint_system_t csts) 
        { CRAB_ERROR (APRON_NOT_FOUND); }
        
        void assign (variable_t x, linear_expression_t e) 
        { CRAB_ERROR (APRON_NOT_FOUND); }
          
        void apply (operation_t op, variable_t x, variable_t y, Number z) 
        { CRAB_ERROR (APRON_NOT_FOUND); }
        
        void apply(operation_t op, variable_t x, variable_t y, variable_t z) 
        { CRAB_ERROR (APRON_NOT_FOUND); }
        
        void apply(operation_t op, variable_t x, Number k) 
        { CRAB_ERROR (APRON_NOT_FOUND); }

        void apply(int_conv_operation_t op, variable_t dst, variable_t src) 
        { CRAB_ERROR (APRON_NOT_FOUND); }
	
        void apply(bitwise_operation_t op, variable_t x, variable_t y, variable_t z) 
        { CRAB_ERROR (APRON_NOT_FOUND); }
        
        void apply(bitwise_operation_t op, variable_t x, variable_t y, Number k) 
        { CRAB_ERROR (APRON_NOT_FOUND); }
        
        void apply(div_operation_t op, variable_t x, variable_t y, variable_t z) 
        { CRAB_ERROR (APRON_NOT_FOUND); }
        
        void apply(div_operation_t op, variable_t x, variable_t y, Number k) 
        { CRAB_ERROR (APRON_NOT_FOUND); }
        
        void backward_assign (variable_t x, linear_expression_t e,
			      apron_domain_t invariant) 
        { CRAB_ERROR (APRON_NOT_FOUND); }
          
        void backward_apply (operation_t op,
			     variable_t x, variable_t y, Number z,
			     apron_domain_t invariant) 
        { CRAB_ERROR (APRON_NOT_FOUND); }
        
        void backward_apply(operation_t op,
			    variable_t x, variable_t y, variable_t z,
			    apron_domain_t invariant) 
        { CRAB_ERROR (APRON_NOT_FOUND); }
        
        linear_constraint_system_t to_linear_constraint_system ()
        { CRAB_ERROR (APRON_NOT_FOUND); }
        
        void write(crab_os& o) 
        { CRAB_ERROR (APRON_NOT_FOUND); }
          
        static std::string getDomainName () {
          return "Dummy Apron";
        }  
      }; 
   } // namespace domains
}// namespace crab
#else

/* 
 *  Real implementation starts here 
 */

#include <crab/domains/apron/apron.hpp>
#include <boost/bimap.hpp>

namespace crab {
   namespace domains {

     using namespace apron;

      template<typename Number, typename VariableName, apron_domain_id_t ApronDom>
      class apron_domain_:
         public abstract_domain<Number,VariableName,
				apron_domain_<Number,VariableName,ApronDom> > {

        typedef apron_domain_<Number, VariableName, ApronDom> apron_domain_t;
	typedef abstract_domain<Number,VariableName,apron_domain_t> abstract_domain_t;
       public:
        using typename abstract_domain_t::linear_expression_t;
        using typename abstract_domain_t::linear_constraint_t;
        using typename abstract_domain_t::linear_constraint_system_t;
        using typename abstract_domain_t::variable_t;
        using typename abstract_domain_t::number_t;
        using typename abstract_domain_t::varname_t;
        using typename abstract_domain_t::variable_vector_t;	
        typedef interval <Number> interval_t;

       private:
        typedef interval_domain <Number, VariableName> interval_domain_t;
        typedef bound <Number> bound_t;
        typedef boost::bimap<variable_t, ap_dim_t > var_map_t;
        typedef typename var_map_t::value_type binding_t;

        static ap_manager_t* m_apman;
        
        ap_state_ptr m_apstate; 
        var_map_t m_var_map;

        static ap_manager_t* get_man () {
          if (!m_apman) {
            switch (ApronDom) {
              case APRON_INT: m_apman = box_manager_alloc (); break;
              case APRON_OCT: m_apman = oct_manager_alloc (); break;
              case (APRON_OPT_OCT): m_apman = opt_oct_manager_alloc (); break;
              case APRON_PK: m_apman = pk_manager_alloc (false); break;
              default: CRAB_ERROR("unknown apron domain");
            }
          }
          return m_apman;
        }

        size_t get_dims (ap_state_ptr s) const {
          ap_dimension_t dims = _ap_abstract0_dimension (&*s);
          return dims.intdim;
        }

        size_t get_dims () const { return get_dims (m_apstate); }

        // If v is in the map then it maps v to a dimension, otherwise null
        boost::optional<ap_dim_t> get_var_dim (const var_map_t& m, variable_t v) const {
          auto it = m.left.find (v);
          if (it != m.left.end ())
            return it->second;
          else
            return boost::optional<ap_dim_t> ();
        }

        boost::optional<ap_dim_t> get_var_dim (variable_t v) const {
          return get_var_dim (m_var_map, v);
        }

        ap_dim_t get_var_dim_insert (variable_t v) {
          assert (m_var_map.size () == get_dims ());
          if (auto dim = get_var_dim (v))
            return *dim;
          else {
            ap_dim_t i = m_var_map.size ();
            m_var_map.insert (binding_t (v, i));
            add_dimensions (m_apstate, 1);
            assert (m_var_map.size () == get_dims ());
            return i;
          }
        }
        
        bool has_variable (const var_map_t& m, ap_dim_t i) const {
          return m.right.find (i) != m.right.end ();
        }

        bool has_variable (ap_dim_t i) const {
          return has_variable (m_var_map, i);
        }

        variable_t get_variable (const var_map_t& m, ap_dim_t i) const {
          auto it = m.right.find (i);
          if (it != m.right.end ())
            return it->second;            
          CRAB_ERROR ("Apron dimension ", i, " is not used!");
        }

        variable_t get_variable (ap_dim_t i) const {
          return get_variable (m_var_map, i);
        }

        void add_dimensions (ap_state_ptr& s, size_t dims) const {
          if (dims <= 0) return;

          ap_dimchange_t* dimchange =  ap_dimchange_alloc (dims, 0);
          for (unsigned i=0 ; i<dims ; i++)
            dimchange->dim[i] = get_dims (s); // add dimension at the end

          s = apPtr (get_man (), 
                     ap_abstract0_add_dimensions(get_man (), false, 
                                                 &*s, dimchange, false));
          ap_dimchange_free (dimchange);
          
        }

        void remove_dimensions (ap_state_ptr& s, std::vector<ap_dim_t> dims) const {
          if (dims.empty ()) return;

          // Apron assumption: make sure that the removing dimensions
          //                   are in ascending order.
          std::sort(dims.begin(), dims.end());

          ap_dimchange_t* dimchange =  ap_dimchange_alloc (dims.size (), 0);
          for (unsigned i=0; i<dims.size () ; i++) {
            // remove dimension dims[i] and shift to the left all the
            // dimensions greater than dims[i]
            dimchange->dim[i] = dims[i]; 
          }

          s = apPtr (get_man (), 
                     ap_abstract0_remove_dimensions(get_man (), false, 
                                                    &*s, dimchange));
          ap_dimchange_free (dimchange);

          #if 0
          crab::outs() << "Removed " << dims.size () << " dimensions\n";
          crab::outs() << "Size = " << get_dims (s) << "\n";
          #endif           
        }

        bool check_perm (ap_dimperm_t* perm, size_t size){
          // it does not check injectivity
          if (perm->size != size) return false;
          for (unsigned i=0; i<perm->size; i++){
            if (perm->dim[i]>=size){
              return false;
            }
          }
          return true;
        }

        var_map_t merge_var_map (const var_map_t& m_x, ap_state_ptr& s_x,
				 const var_map_t& m_y, ap_state_ptr& s_y) {

          assert (m_x.size () == get_dims (s_x));
          assert (m_y.size () == get_dims (s_y));

          // -- collect all vars from the two maps
          std::set<variable_t> vars;
          for (auto const& px: m_x.left)
            vars.insert (px.first);
          for (auto const& py: m_y.left)
            vars.insert (py.first);

          assert (vars.size () >= get_dims (s_x));
          assert (vars.size () >= get_dims (s_y));

          add_dimensions (s_x, vars.size () - get_dims (s_x));
          add_dimensions (s_y, vars.size () - get_dims (s_y));

          assert (get_dims (s_x) == get_dims (s_y));

          // -- create a fresh map 
          var_map_t res;
          for (auto v: vars) {
            ap_dim_t i = res.size ();
            assert (i < get_dims (s_x));
            res.insert (binding_t (v, i));
          }
          
          // build the permutations maps
          ap_dimperm_t* perm_x = ap_dimperm_alloc (get_dims (s_x));
          ap_dimperm_t* perm_y = ap_dimperm_alloc (get_dims (s_x));
          char * xmap1 = (char *)calloc (get_dims (s_x), sizeof(char));
          if (!xmap1)
	    CRAB_ERROR ("calloc does not have more available memory");
          char * xmap2 = (char *)calloc (get_dims (s_x), sizeof(char));
          if (!xmap2)
	    CRAB_ERROR ("calloc does not have more available memory");
          for (auto const &px: m_x.left) {
            ap_dim_t ind = res.left.at (px.first);
            perm_x->dim [px.second] = ind;
            // This sets 1 if the index that has been assigned
            assert (px.second < get_dims(s_x));
            xmap1[px.second] = 1;
            // This sets 1 if the value has been assigned
            assert (ind < get_dims(s_x));
            xmap2[ind] = 1;
          }
          ap_dim_t i, counter = 0;
          for(i=0; i < get_dims (s_x); i++){
            // If the index has been assigned, skip
            if(xmap1[i]) continue;
            // Find the next available element that has not been assigned
            while(xmap2[counter])
              counter++;
            perm_x->dim[i] = counter;
            counter++;
          }
          free (xmap1);
          free (xmap2);

          char * ymap1 = (char *)calloc (get_dims (s_x), sizeof(char));
          if (!ymap1) CRAB_ERROR ("calloc does not have more available memory");
          char * ymap2 = (char *)calloc (get_dims (s_x), sizeof(char));
          if (!ymap2) CRAB_ERROR ("calloc does not have more available memory");
          for (auto const &py: m_y.left) {
            ap_dim_t ind = res.left.at (py.first);
            perm_y->dim [py.second] = ind;
            assert (py.second < get_dims(s_x));
            ymap1[py.second] = 1;
            assert (ind < get_dims(s_x));
            ymap2[ind] = 1; 
          }

          counter = 0;
          for(i=0; i < get_dims (s_x); i++){
            if(ymap1[i]) continue;
            while(ymap2[counter])
              counter++;
            perm_y->dim[i] = counter;
            counter++;
          }

          free (ymap1);
          free (ymap2);

          #if 0          
          crab::outs() << "Permutations \n";
          ap_dimperm_fprint(stdout, perm_x);          
          crab::outs() << "Permutations \n";
          ap_dimperm_fprint(stdout, perm_y);          
          #endif 

          assert (check_perm (perm_x, get_dims (s_x)));
          assert (check_perm (perm_y, get_dims (s_x)));

          // apply the permutations
          s_x = apPtr (get_man (), 
                       ap_abstract0_permute_dimensions(get_man (), false, &*s_x, perm_x));
          s_y = apPtr (get_man (), 
                       ap_abstract0_permute_dimensions(get_man (), false, &*s_y, perm_y));

          ap_dimperm_free (perm_x);
          ap_dimperm_free (perm_y);

          assert (res.size () == get_dims (s_x));
          assert (res.size () == get_dims (s_y));

          return res;
        }
	       	
	// FIXME: dispatch based on Number ?
        #if 0
	static ap_texpr0_t* ADD(ap_texpr0_t* a, ap_texpr0_t*b)
	{ /// XXX: should not be the rounding direction AP_RDIR_ZERO 
	  ///      (i.e., truncation for integers) ?
          return ap_texpr0_binop(AP_TEXPR_ADD,a,b,AP_RTYPE_INT,AP_RDIR_NEAREST);
	}
	static ap_texpr0_t* SUB(ap_texpr0_t* a, ap_texpr0_t*b)
	{ return ap_texpr0_binop(AP_TEXPR_SUB,a,b,AP_RTYPE_INT,AP_RDIR_NEAREST);}	  
	static ap_texpr0_t* MUL(ap_texpr0_t* a, ap_texpr0_t*b)
	{ return ap_texpr0_binop(AP_TEXPR_MUL,a,b,AP_RTYPE_INT,AP_RDIR_NEAREST); }	  
	static ap_texpr0_t* DIV(ap_texpr0_t* a, ap_texpr0_t*b)
	{ return ap_texpr0_binop(AP_TEXPR_DIV,a,b,AP_RTYPE_INT,AP_RDIR_NEAREST);}	  	
	#else
	///////////
	// XXX: we approximate integers and rationals using reals
	//////////
	
        static ap_texpr0_t* ADD(ap_texpr0_t* a, ap_texpr0_t*b)
	{ /// XXX: AP_RTYPE_REAL does not have rounding so we choose
	  ///      an arbitrary one
          return ap_texpr0_binop(AP_TEXPR_ADD,a,b,AP_RTYPE_REAL,AP_RDIR_UP);
	}
        static ap_texpr0_t* SUB(ap_texpr0_t* a, ap_texpr0_t*b)	  
	{ return ap_texpr0_binop(AP_TEXPR_SUB,a,b,AP_RTYPE_REAL,AP_RDIR_UP);}
        static ap_texpr0_t* MUL(ap_texpr0_t* a, ap_texpr0_t*b)	  
	{ return ap_texpr0_binop(AP_TEXPR_MUL,a,b,AP_RTYPE_REAL,AP_RDIR_UP);}
        static ap_texpr0_t* DIV(ap_texpr0_t* a, ap_texpr0_t*b)	  
	{ return ap_texpr0_binop(AP_TEXPR_DIV,a,b,AP_RTYPE_REAL,AP_RDIR_UP);}
	#endif
	       	
        // --- from crab to apron
	
        inline ap_texpr0_t* var2texpr (variable_t v) { 
          return ap_texpr0_dim (get_var_dim_insert (v));
        }

	inline void convert_crab_number (ikos::z_number n, mpq_class &res) const
	{ res = mpq_class ((mpz_class) n); }

	inline void convert_crab_number (ikos::q_number n, mpq_class &res) const 
	{ res = ((mpq_class) n); }
		
        inline ap_texpr0_t* num2texpr (Number i) const {  
	  mpq_class n(0);
	  convert_crab_number(i, n);
          return ap_texpr0_cst_scalar_mpq (n.get_mpq_t ());
        }

        inline ap_texpr0_t* intv2texpr (Number a, Number b) const {
	  mpq_class n1(0), n2(0);
	  convert_crab_number(a, n1);
	  convert_crab_number(b, n2);
          return ap_texpr0_cst_interval_mpq (n1.get_mpq_t (), n2.get_mpq_t ());
        }
        
        inline ap_texpr0_t* expr2texpr (linear_expression_t e)  {
          Number cst = e.constant ();
          ap_texpr0_t* res = num2texpr (cst);
          for (auto p: e) {
            ap_texpr0_t* term = MUL (num2texpr(p.first), var2texpr(p.second));
            res = ADD (res, term); 
          }
          return res;
        }

        inline ap_tcons0_t const2tconst (linear_constraint_t cst)  {
          linear_expression_t exp = cst.expression();
          if (cst.is_equality ()) {
            return ap_tcons0_make (AP_CONS_EQ, expr2texpr (exp), NULL);            
          } else if (cst.is_inequality ()) {
            return ap_tcons0_make (AP_CONS_SUPEQ, expr2texpr (-exp), NULL);
          } else if (cst.is_strict_inequality()) {
            return ap_tcons0_make (AP_CONS_SUP, expr2texpr (-exp), NULL);
	  } else  {
	    assert(cst.is_disequation ());
            return ap_tcons0_make (AP_CONS_DISEQ, expr2texpr (exp), NULL);
          } 
        }
		
        // --- from apron to crab 

	inline void convert_apron_number (double n, ikos::z_number &res) const
	{ res = ikos::z_number ((long) n); }

	inline void convert_apron_number (double n, ikos::q_number &res) const
	{ res = ikos::q_number (n); }
	
	inline void convert_apron_number (mpq_ptr n, ikos::z_number &res) const
	{ // FIXME: find a better way
	  res = ikos::z_number (mpz_class(mpq_class(n)));
	}
	inline void convert_apron_number (mpq_ptr n, ikos::q_number &res) const
	{ res = ikos::q_number (mpq_class(n)); }	  

        Number coeff2Num (ap_coeff_t* coeff) {
          assert (coeff->discr == AP_COEFF_SCALAR);

	  ap_scalar_t* scalar = coeff->val.scalar;	  
          if (scalar->discr == AP_SCALAR_DOUBLE) { // elina uses double
	    Number res;
	    convert_apron_number(scalar->val.dbl, res);
	    return res;
          }
          else if (scalar->discr == AP_SCALAR_MPQ) {
	    Number res;
	    convert_apron_number(scalar->val.mpq, res);
	    return res;
          }
          else
            CRAB_ERROR ("apron translation only covers double or mpq scalars");
        }

        linear_expression_t term2expr (ap_coeff_t* coeff, ap_dim_t i) {
          return variable_t (get_variable (i)) * coeff2Num(coeff) ;
        }

        linear_constraint_t tconst2const (ap_lincons0_t cons) {
          assert (cons.scalar == NULL); // Not modulo form
          ap_linexpr0_t* linexp = cons.linexpr0;
          assert (ap_linexpr0_is_linear (linexp));

          linear_expression_t e (0);
          for (unsigned i=0; i < get_dims (); ++i) {
            ap_coeff_t* coeff = ap_linexpr0_coeffref (linexp, i);
            if (ap_coeff_zero (coeff)) continue;
            
            if (!has_variable (i)) continue; // unused dimension

            e = e + term2expr ( coeff, i);
          }

          // add possible constant
          ap_coeff_t* cst = ap_linexpr0_cstref (linexp);
          if (!ap_coeff_zero (cst)) 
            e = e + coeff2Num(cst);
          linear_constraint_t res;
          switch (cons.constyp) {
            case AP_CONS_EQ:
              // e == k 
              res =  linear_constraint_t (e, linear_constraint_t::kind_t::EQUALITY);
              break;
            case AP_CONS_SUPEQ:
              // e >= k 
              e = -e;
              res =  linear_constraint_t (e, linear_constraint_t::kind_t::INEQUALITY);
              break;
            case AP_CONS_SUP:
              // e > k 
              e = -e;
              res =  linear_constraint_t (e, linear_constraint_t::kind_t::STRICT_INEQUALITY);
              break;
            case AP_CONS_EQMOD:
              res = T ();
              break;
            case AP_CONS_DISEQ:
              // e != k
              res =  linear_constraint_t (e, linear_constraint_t::kind_t::DISEQUATION);
              break;
          }
          return res;
        }

        inline linear_constraint_t T () const {
          return linear_constraint_t (linear_expression_t (Number(1)) == 
                                      linear_expression_t (Number(1)));          
        }

        inline linear_constraint_t F () const {
          return linear_constraint_t (linear_expression_t (Number(1)) == 
                                      linear_expression_t (Number(0)));          
        }

        void dump (const var_map_t& m, ap_state_ptr apstate ) {  
          crab::outs() << "\nNumber of dimensions=" << get_dims (apstate) << "\n";
          crab::outs() << "variable map ["; 
          std::vector<char*> names;
          for (unsigned i=0; i < get_dims (apstate) ; i++){
            std::string varname;
            if (has_variable (m, i))
              varname = get_variable (m, i).str ();
            else // unused dimension
              varname = std::string ("_x") + std::to_string (i);
            crab::outs() << i << " -> " << varname << ";";
            char* name = new char [varname.length () + 1];
            strcpy (name, varname.c_str ());
            names.push_back (name);
          }
          crab::outs() << "]\n";
          ap_abstract0_fprint (stdout, get_man (), &*apstate, &names[0]);
          for (auto n : names) { delete n; }
        }

        void dump () { dump (m_var_map, m_apstate); }

       public:
        void print_stats () { ap_abstract0_fprint (stdout, get_man (), &*m_apstate, NULL); }

       private:

        apron_domain_ (ap_state_ptr apState, var_map_t varMap):
	  m_apstate (apState), m_var_map (varMap) {
	  
          std::vector<ap_dim_t> dims;
          var_map_t res;
          /// XXX: we must iterate on the dimension id's to preserve
          /// order between them
          for (auto const& p: m_var_map.right) {  
            if (ap_abstract0_is_dimension_unconstrained (get_man (),
                                                         &*m_apstate, 
                                                         p.first)) {
              dims.push_back (p.first);
            }
            else {
              ap_dim_t i = res.size ();
              res.insert (binding_t (p.second, i));
            }
          }
          remove_dimensions (m_apstate, dims);
          std::swap (m_var_map, res);

          assert (m_var_map.size () == get_dims ());
        }


        apron_domain_ (ap_state_ptr&& apState, var_map_t&& varMap):
	  m_apstate (std::move (apState)), 
	  m_var_map (std::move (varMap)) { 

          std::vector<ap_dim_t> dims;
          var_map_t res;
          /// XXX: we must iterate on the dimension id's to preserve
          /// order between them
          for (auto const& p: m_var_map.right) {  
            if (ap_abstract0_is_dimension_unconstrained (get_man (),
                                                         &*m_apstate, 
                                                         p.first)) {
              dims.push_back (p.first);
            }
            else {
              ap_dim_t i = res.size ();
              res.insert (binding_t (p.second, i));
            }
          }
          remove_dimensions (m_apstate, dims);
          std::swap (m_var_map, res);

          assert (m_var_map.size () == get_dims ());
        }


       public:

        apron_domain_ (bool isBot = false):
	  m_apstate (apPtr (get_man(), 
			    (isBot ? 
			     ap_abstract0_bottom (get_man(), 0, 0) : 
			     ap_abstract0_top (get_man(), 0, 0)))) {}

        ~apron_domain_ () {}

        apron_domain_ (const apron_domain_t& o): 
            m_apstate (apPtr (get_man (), ap_abstract0_copy (get_man (), &*(o.m_apstate)))),
            m_var_map (o.m_var_map)
        {  
          crab::CrabStats::count (getDomainName() + ".count.copy");
          crab::ScopedCrabStats __st__(getDomainName() + ".copy");
        }

        apron_domain_ (apron_domain_t&& o): 
            m_apstate (std::move (o.m_apstate)), 
            m_var_map (std::move (o.m_var_map)) { }
        
        apron_domain_t& operator=(const apron_domain_t& o) {
          crab::CrabStats::count (getDomainName() + ".count.copy");
          crab::ScopedCrabStats __st__(getDomainName() + ".copy");
          if (this != &o) {
            m_apstate = o.m_apstate;
            m_var_map = o.m_var_map;
          }
          return *this;
        }

        apron_domain_t& operator=(apron_domain_t&& o) {
          if (this != &o) {
            m_apstate = std::move (o.m_apstate);
            m_var_map = std::move (o.m_var_map);
          }
          return *this;
        }
        
        static apron_domain_t top() { 
          return apron_domain_t (false);
        }

        static apron_domain_t bottom() { 
          return apron_domain_t (true);
        }

        bool is_bottom() { 
          return ap_abstract0_is_bottom (get_man(), &*m_apstate);
        }

        bool is_top() { 
          return ap_abstract0_is_top (get_man(), &*m_apstate);
        }

        bool operator<=(apron_domain_t o) { 
          crab::CrabStats::count (getDomainName() + ".count.leq");
          crab::ScopedCrabStats __st__(getDomainName() + ".leq");

          if (is_bottom()) 
            return true;
          else if(o.is_bottom())
            return false;
          else if (o.is_top ())
            return true;
          else if (is_top () && !o.is_top ())
            return false;
          else if (is_top () && o.is_top ())
            return true;
          else { 
            ap_state_ptr x = apPtr (get_man (), ap_abstract0_copy (get_man (), &*m_apstate));
            merge_var_map (m_var_map, x, o.m_var_map, o.m_apstate);
            return ap_abstract0_is_leq (get_man(), &*x, &*o.m_apstate);
          }
        }

        void operator|=(apron_domain_t o) {
          crab::CrabStats::count (getDomainName() + ".count.join");
          crab::ScopedCrabStats __st__(getDomainName() + ".join");

          if (is_bottom() || o.is_top ())
            *this = o;
          else if (is_top () || o.is_bottom())
            return ;
          else {
            m_var_map = merge_var_map (m_var_map, m_apstate, o.m_var_map, o.m_apstate);
            m_apstate = apPtr (get_man(), 
                               ap_abstract0_join (get_man(), false, 
                                                  &*m_apstate, &*o.m_apstate));
          }
        }
        
        apron_domain_t operator|(apron_domain_t o) {
          crab::CrabStats::count (getDomainName() + ".count.join");
          crab::ScopedCrabStats __st__(getDomainName() + ".join");

          if (is_bottom() || o.is_top ())
            return o;
          else if (is_top () || o.is_bottom())
            return *this;
          else {
            ap_state_ptr x = apPtr (get_man (), ap_abstract0_copy (get_man (), &*m_apstate));
            var_map_t  m = merge_var_map (m_var_map, x, o.m_var_map, o.m_apstate);
            return apron_domain_t (apPtr (get_man(), 
                                          ap_abstract0_join (get_man(), false, 
                                                             &*x, &*o.m_apstate)), m);
          }
        }        
        
        apron_domain_t operator&(apron_domain_t o) {
          crab::CrabStats::count (getDomainName() + ".count.meet");
          crab::ScopedCrabStats __st__(getDomainName() + ".meet");

          if (is_bottom() || o.is_bottom())
            return bottom();
          else if (is_top())
            return o;
          else if (o.is_top())
            return *this;
          else{
            ap_state_ptr x = apPtr (get_man (), ap_abstract0_copy (get_man (), &*m_apstate));
            var_map_t  m = merge_var_map (m_var_map, x, o.m_var_map, o.m_apstate);
            return apron_domain_t (apPtr (get_man(), 
                                          ap_abstract0_meet (get_man(), false, 
                                                             &*x, &*o.m_apstate)), m);
          }
        }        
        
        apron_domain_t operator||(apron_domain_t o) {
          crab::CrabStats::count (getDomainName() + ".count.widening");
          crab::ScopedCrabStats __st__(getDomainName() + ".widening");

          if (is_bottom())
            return o;
          else if (o.is_bottom())
            return *this;
          else {
            ap_state_ptr x = apPtr (get_man (), ap_abstract0_copy (get_man (), &*m_apstate));
            var_map_t  m = merge_var_map (m_var_map, x, o.m_var_map, o.m_apstate);
            return apron_domain_t (apPtr (get_man(), 
                                          ap_abstract0_widening (get_man(), 
                                                                 &*x, &*o.m_apstate)), m);
          }
        }        

        template<typename Thresholds>	
	ap_lincons0_array_t make_thresholds (apron_domain_t o, const Thresholds &ts) {
	  // TODO: make some constraints using the constants from ts
	  ap_lincons0_array_t csts = ap_lincons0_array_make(0);
	  return csts;
	}
	
        template<typename Thresholds>
        apron_domain_t widening_thresholds (apron_domain_t o, const Thresholds &ts) {
          crab::CrabStats::count (getDomainName() + ".count.widening");
          crab::ScopedCrabStats __st__(getDomainName() + ".widening");

          if (is_bottom())
            return o;
          else if (o.is_bottom())
            return *this;
          else {
            ap_state_ptr x = apPtr (get_man (), ap_abstract0_copy (get_man (), &*m_apstate));
            var_map_t  m = merge_var_map (m_var_map, x, o.m_var_map, o.m_apstate);	    
	    #if 1
	    // widening w/o thresholds in the apron domain
            apron_domain_t res (apPtr (get_man(), 
					 ap_abstract0_widening (get_man(), 
								&*x, &*o.m_apstate)), m);

	    // widening w/ thresholds in the interval domain
	    auto intv_this  = this->to_interval_domain ();
	    auto intv_o     = o.to_interval_domain ();
	    auto intv_widen = intv_this.widening_thresholds (intv_o, ts);	    
	    
	    // refine the apron domain using the widen intervals
	    apron_domain_t apron_intv_widen;
	    apron_intv_widen += intv_widen.to_linear_constraint_system ();
	    return res & apron_intv_widen;
	    #else
	    ap_lincons0_array_t csts = make_thresholds (o, ts);
            apron_domain_t res (apPtr (get_man(), 
				       ap_abstract0_widening_threshold
				       (get_man(), &*x, &*o.m_apstate, &csts)), m);
	    ap_lincons0_array_clear (&csts);
	    return res;
	    #endif 
          }
        }

        apron_domain_t operator&&(apron_domain_t o) {
          crab::CrabStats::count (getDomainName() + ".count.narrowing");
          crab::ScopedCrabStats __st__(getDomainName() + ".narrowing");

          if (is_bottom() || o.is_bottom())
            return bottom();
          else if (is_top())
            return o;
          else if (o.is_top())
            return *this;
          else{
            ap_state_ptr x = apPtr (get_man (), ap_abstract0_copy (get_man (), &*m_apstate));
            var_map_t  m = merge_var_map (m_var_map, x, o.m_var_map, o.m_apstate);
            switch (ApronDom) {
              case APRON_OCT:
                return apron_domain_t (apPtr (get_man(), 
                                              ap_abstract0_oct_narrowing (get_man(),
                                                                          &*x, &*o.m_apstate)), m);
              case APRON_OPT_OCT:
                return apron_domain_t (apPtr (get_man(), 
                                              ap_abstract0_opt_oct_narrowing (get_man(),
                                                                              &*x, &*o.m_apstate)), m);
              case APRON_INT:
              case APRON_PK:
              default:
                //CRAB_WARN ("used meet instead of narrowing: \n",
		//           "make sure only a finite number of descending iterations are run.");
                return apron_domain_t (apPtr (get_man(), 
                                              ap_abstract0_meet (get_man(), false,
								 &*x, &*o.m_apstate)), m);
            }
          }
        }        

        template<typename Range>
        void forget (const Range &vars) {
          crab::CrabStats::count (getDomainName() + ".count.forget");
          crab::ScopedCrabStats __st__(getDomainName() + ".forget");

          std::vector<ap_dim_t> vector_dims;
          std::set<ap_dim_t> set_dims;

          for (auto v: vars)  {
            if (auto dim = get_var_dim (v)) {
              vector_dims.push_back (*dim);
              set_dims.insert (*dim);
            }
          }

          if (vector_dims.empty ()) return;

          m_apstate = apPtr (get_man (), 
                             ap_abstract0_forget_array (get_man (), 
                                                        false, 
                                                        &*m_apstate, 
                                                        &vector_dims[0], vector_dims.size(), 
                                                        false));

          // -- Remove forgotten dimensions while compacting
          var_map_t res;
          /// XXX: we must iterate on the dimension id's to preserve
          /// order between them
          for (auto const& p: m_var_map.right) {  
             if (set_dims.count (p.first) <= 0) {
               ap_dim_t i = res.size ();
               res.insert (binding_t (p.second, i));
             }
          }

          remove_dimensions (m_apstate, vector_dims);
          std::swap (m_var_map, res);
        }

        void operator-=(variable_t var) {
          std::vector<ap_dim_t> vector_dims;
          if (auto dim = get_var_dim (var)) {
            vector_dims.push_back (*dim);
            m_apstate = apPtr (get_man (), 
                               ap_abstract0_forget_array (get_man (), 
                                                          false, 
                                                          &*m_apstate, 
                                                          &vector_dims[0], vector_dims.size(), 
                                                          false));
            // -- Remove forgotten dimensions while compacting
            var_map_t res;
            /// XXX: we must iterate on the dimension id's to preserve
            /// order between them
            for (auto const& p: m_var_map.right) {  
              if (p.first != *dim) {
                ap_dim_t i = res.size ();
                res.insert (binding_t (p.second, i));
              }
            }
            remove_dimensions (m_apstate, vector_dims);
            std::swap (m_var_map, res);
          }
        }

        // remove all variables except vars
        template<typename Range>
        void project (const Range& vars) {
          crab::CrabStats::count (getDomainName() + ".count.project");
          crab::ScopedCrabStats __st__(getDomainName() + ".project");

          if (is_bottom ()) return;
          std::set<variable_t> s1,s2,s3;
          for (auto p: m_var_map.left) s1.insert (p.first);
          s2.insert (vars.begin (), vars.end ());
          boost::set_difference (s1,s2,std::inserter (s3, s3.end ()));
          forget (s3);
        }

        interval_t operator[](variable_t v) {
          crab::CrabStats::count (getDomainName() + ".count.to_intervals");
          crab::ScopedCrabStats __st__(getDomainName() + ".to_intervals");

          if (is_bottom ()) 
            return interval_t::bottom ();

          if (auto dim = get_var_dim (v)) {

            ap_interval_t* intv = ap_abstract0_bound_dimension (get_man (),
                                                                &*m_apstate, 
                                                                *dim);
            if (ap_interval_is_top (intv)) {
	      ap_interval_free (intv);
              return interval_t::top ();
	    }

            ap_scalar_t* lb = intv->inf;
            ap_scalar_t* ub = intv->sup;
            
            if (lb->discr == AP_SCALAR_DOUBLE && ub->discr == AP_SCALAR_DOUBLE) { 

              if (ap_scalar_infty(lb) == -1) {     // [-oo, k]
		Number sup;
		convert_apron_number (ub->val.dbl, sup);
		ap_interval_free (intv);
                return interval_t (bound_t::minus_infinity (), sup);
              }
              else if (ap_scalar_infty(ub) == 1) { // [k, +oo]
		Number inf;
		convert_apron_number (lb->val.dbl, inf);
		ap_interval_free (intv);
                return interval_t (inf, bound_t::plus_infinity ());
                                   
              }
              else { 
                assert (ap_scalar_infty(lb) == 0); // lb is finite
                assert (ap_scalar_infty(ub) == 0); // ub is finite
		Number inf, sup;
		convert_apron_number(lb->val.dbl, inf);
		convert_apron_number(ub->val.dbl, sup);
		ap_interval_free (intv);
                return interval_t (inf, sup);
              }

            }
            else if (lb->discr == AP_SCALAR_MPQ && ub->discr == AP_SCALAR_MPQ ) {

              if (ap_scalar_infty(lb) == -1) {     // [-oo, k]
		Number sup;
		convert_apron_number (ub->val.mpq, sup);
		ap_interval_free (intv);
                return interval_t (bound_t::minus_infinity (), sup);

              }
              else if (ap_scalar_infty(ub) == 1) { // [k, +oo]
		Number inf;
		convert_apron_number (lb->val.mpq, inf);
		ap_interval_free (intv);
                return interval_t (inf, bound_t::plus_infinity ());
              }
              else {
                assert (ap_scalar_infty(lb) == 0); // lb is finite
                assert (ap_scalar_infty(ub) == 0); // ub is finite

		Number inf, sup;
		convert_apron_number (lb->val.mpq, inf);
		convert_apron_number (ub->val.mpq, sup);		
		ap_interval_free (intv);
                return interval_t (inf, sup);
              }
            }
            else 
              CRAB_ERROR ("apron translation only covers double or mpq scalars");
          }
          else 
            return interval_t::top ();
        }

        void set(variable_t v, interval_t ival) {
          crab::CrabStats::count (getDomainName() + ".count.assign");
          crab::ScopedCrabStats __st__(getDomainName() + ".assign");

	  variable_t vv(v);
	  
          // -- forget v
          *this -= v;

          // -- add constraints v >= lb and v <= ub
          linear_constraint_system_t csts;
          auto lb = ival.lb ();
          if (lb.is_finite ())  {
            // v >= lb <--> -v + lb <= 0
            assert (lb.number ());
            linear_expression_t e = (Number(-1) * vv) + *(lb.number ());
            csts += (linear_constraint_t (e, linear_constraint_t::kind_t::INEQUALITY));
          }
          auto ub = ival.ub ();
          if (ub.is_finite ()) {
            // v <= ub <--> v - ub <= 0
            assert (ub.number ());
            linear_expression_t e = (vv - *(ub.number ()));
            csts += (linear_constraint_t (e, linear_constraint_t::kind_t::INEQUALITY));
          }

          if (csts.size () > 0)
            *this += csts;
        }

        void operator+=(linear_constraint_system_t _csts) {
          crab::CrabStats::count (getDomainName() + ".count.add_constraints");
          crab::ScopedCrabStats __st__(getDomainName() + ".add_constraints");

          if(is_bottom()) return;

	  if (_csts.is_false()) {
	    *this = bottom ();
	    return;
	  }

	  // XXX: filter out unsigned linear inequalities
	  linear_constraint_system_t csts;
	  bool has_disequality = false;
	  for (auto const& c: _csts) {
	    if (c.is_inequality() && c.is_unsigned()) {
	      CRAB_WARN("unsigned inequality skipped");
	      continue;
	    }
	    if (c.is_disequation()) {
	      has_disequality = true;
	    }
	    csts += c;
	  }

	  if (has_disequality) {
	    // trivial reduction between the apron domain and
	    // intervals. This is done because most of the apron
	    // domains ignore disequalities. Of course, more things
	    // can be done here to improve precision.
	    interval_domain_t intvs;
	    intvs = to_interval_domain();
	    intvs += csts;
	    if (intvs.is_bottom()) {
	      *this = bottom ();
	      return;
	    }
	  }

          ap_tcons0_array_t array = ap_tcons0_array_make (csts.size ());
          unsigned i=0;

          for (auto cst : csts) {
            ap_tcons0_t tcons = const2tconst (cst);
            array.p[i] = tcons;
            ++i;
          }

	  #if 0
	  ///// debugging
	  std::vector<char*> names;
	  for (unsigned i=0; i < get_dims (m_apstate) ; i++){
	    std::string varname;
	    if (has_variable (m_var_map, i))
	      varname = get_variable (m_var_map, i).str ();
	    else // unused dimension
	      varname = std::string ("_x") + std::to_string (i);
	    char* name = new char [varname.length () + 1];
	    strcpy (name, varname.c_str ());
	    names.push_back (name);
	  }
	  ap_tcons0_array_fprint (stdout, &array, &names[0]);
	  for (auto n : names) { delete n; }
	  #endif 
	  
          m_apstate = apPtr (get_man (), 
                             ap_abstract0_meet_tcons_array (get_man (), false, 
                                                            &*m_apstate, &array));

          ap_tcons0_array_clear(&array);
          CRAB_LOG("apron", 
                   crab::outs() << "--- " << "Assume " << csts << " --> "
		                 << *this << "\n";);
        }
       
        void assign (variable_t x, linear_expression_t e) {
          crab::CrabStats::count (getDomainName() + ".count.assign");
          crab::ScopedCrabStats __st__(getDomainName() + ".assign");

          if(is_bottom()) return;

          ap_texpr0_t* t = expr2texpr (e);
          assert (t);
          auto dim_x = get_var_dim_insert (x);
          m_apstate = apPtr (get_man (), 
                             ap_abstract0_assign_texpr(get_man (), false, 
                                                       &*m_apstate, 
                                                       dim_x, t, NULL));
                                                       
          ap_texpr0_free (t);
          CRAB_LOG("apron",
                   crab::outs() << "--- "<< x<< ":="<< e << " --> "<< *this<<"\n";);
        }
          
        void apply (operation_t op, variable_t x, variable_t y, Number z) {
          crab::CrabStats::count (getDomainName() + ".count.apply");
          crab::ScopedCrabStats __st__(getDomainName() + ".apply");

          if(is_bottom()) return;

          ap_texpr0_t* a = var2texpr (y);
          ap_texpr0_t* b = num2texpr (z);
          ap_texpr0_t* res = nullptr;
          
          switch (op){
            case OP_ADDITION: res = ADD (a, b); break;
            case OP_SUBTRACTION: res = SUB (a, b); break;
            case OP_MULTIPLICATION: res = MUL (a, b); break;
            case OP_DIVISION: res = DIV (a, b); break;
            default: CRAB_ERROR("apron operation not supported");
          }
          assert (res);

          auto dim_x = get_var_dim_insert (x);
          m_apstate = apPtr (get_man (), ap_abstract0_assign_texpr(get_man (), false, 
                                                                   &*m_apstate, 
                                                                   dim_x, res, NULL));
                                                                   
          ap_texpr0_free (res);
          CRAB_LOG("apron",
                   crab::outs() << "--- "<< x<< ":="<< y<< op<< z<< " --> "<< *this<<"\n";);
        }
        
        void apply(operation_t op, variable_t x, variable_t y, variable_t z) {
          crab::CrabStats::count (getDomainName() + ".count.apply");
          crab::ScopedCrabStats __st__(getDomainName() + ".apply");

          if(is_bottom()) return;

          ap_texpr0_t* a = var2texpr (y);
          ap_texpr0_t* b = var2texpr (z);
          ap_texpr0_t* res = nullptr;
          
          switch (op){
            case OP_ADDITION: res = ADD (a, b); break;
            case OP_SUBTRACTION: res = SUB (a, b); break;
            case OP_MULTIPLICATION: res = MUL (a, b); break;
            case OP_DIVISION: res = DIV (a, b); break;
            default: CRAB_ERROR("apron operation not supported");
          }
          assert (res);

          auto dim_x = get_var_dim_insert (x);
          m_apstate = apPtr (get_man (),
			     ap_abstract0_assign_texpr(get_man (), false, 
						       &*m_apstate, 
						       dim_x, res, NULL));
	  
          ap_texpr0_free (res);
          CRAB_LOG("apron",
                   crab::outs() << "--- "<< x<< ":="<< y<< op<< z<< " --> "<< *this<<"\n";);
        }
        
        void apply(operation_t op, variable_t x, Number k) {
          crab::CrabStats::count (getDomainName() + ".count.apply");
          crab::ScopedCrabStats __st__(getDomainName() + ".apply");

          if(is_bottom()) return;

          ap_texpr0_t* a = var2texpr (x);
          ap_texpr0_t* b = num2texpr (k);
          ap_texpr0_t* res = nullptr;
          
          switch (op){
            case OP_ADDITION: res = ADD (a, b); break;
            case OP_SUBTRACTION: res = SUB (a, b); break;
            case OP_MULTIPLICATION: res = MUL (a, b); break;
            case OP_DIVISION: res = DIV (a, b); break;
            default: CRAB_ERROR("apron operation not supported");
          }
          assert (res);

          auto dim_x = get_var_dim_insert (x);
          m_apstate = apPtr (get_man (),
			     ap_abstract0_assign_texpr(get_man (), false, 
						       &*m_apstate, 
						       dim_x, res, NULL));
	  
          ap_texpr0_free (res);
          CRAB_LOG("apron",
                   crab::outs() << "--- "<< x<< ":="<< x<< op<< k<< " --> "<< *this<<"\n";);
        }

        void apply(int_conv_operation_t op, variable_t dst, variable_t src) {
          // since reasoning about infinite precision we simply assign and
          // ignore the widths.
          assign(dst, src);
	}

        void apply(bitwise_operation_t op, variable_t x, variable_t y, variable_t z) {
          crab::CrabStats::count (getDomainName() + ".count.apply");
          crab::ScopedCrabStats __st__(getDomainName() + ".apply");

          // Convert to intervals and perform the operation
          interval_t yi = operator[](y);
          interval_t zi = operator[](z);
          interval_t xi = interval_t::top();
          switch (op) {
            case OP_AND: xi = yi.And(zi); break;
            case OP_OR: xi = yi.Or(zi); break;
            case OP_XOR: xi = yi.Xor(zi); break; 
            case OP_SHL: xi = yi.Shl(zi); break; 
            case OP_LSHR: xi = yi.LShr(zi); break;
            case OP_ASHR: xi = yi.AShr(zi); break;
            default: CRAB_ERROR("apron operation not supported");
          }
          set(x, xi);
        }
        
        void apply(bitwise_operation_t op, variable_t x, variable_t y, Number k) {
          crab::CrabStats::count (getDomainName() + ".count.apply");
          crab::ScopedCrabStats __st__(getDomainName() + ".apply");

          // Convert to intervals and perform the operation
          interval_t yi = operator[](y);
          interval_t zi(k);
          interval_t xi = interval_t::top();
          switch (op) {
            case OP_AND: xi = yi.And(zi); break;
            case OP_OR: xi = yi.Or(zi); break;
            case OP_XOR: xi = yi.Xor(zi); break; 
            case OP_SHL: xi = yi.Shl(zi); break; 
            case OP_LSHR: xi = yi.LShr(zi); break;
            case OP_ASHR: xi = yi.AShr(zi); break;
            default: CRAB_ERROR("apron operation not supported");
          }
          set(x, xi);
        }
        
        void apply(div_operation_t op, variable_t x, variable_t y, variable_t z) {
          crab::CrabStats::count (getDomainName() + ".count.apply");
          crab::ScopedCrabStats __st__(getDomainName() + ".apply");

          if (op == OP_SDIV){
            apply(OP_DIVISION, x, y, z);
          }
          else {
            // Convert to intervals and perform the operation
            interval_t yi = operator[](y);
            interval_t zi = operator[](z);
            interval_t xi = interval_t::top ();
            
            switch (op) {
              case OP_UDIV: xi = yi.UDiv(zi); break;
              case OP_SREM: xi = yi.SRem(zi); break;
              case OP_UREM: xi = yi.URem(zi); break;
              default: CRAB_ERROR("apron operation not supported");
            }
            set(x, xi);
          }
        }
        
        void apply(div_operation_t op,
		   variable_t x, variable_t y, Number k) {
          crab::CrabStats::count (getDomainName() + ".count.apply");
          crab::ScopedCrabStats __st__(getDomainName() + ".apply");

          if (op == OP_SDIV){
            apply(OP_DIVISION, x, y, k);
          }
          else {
            // Convert to intervals and perform the operation
            interval_t yi = operator[](y);
            interval_t zi(k);
            interval_t xi = interval_t::top ();
            switch (op) {
              case OP_UDIV: xi = yi.UDiv(zi); break;
              case OP_SREM: xi = yi.SRem(zi); break;
              case OP_UREM: xi = yi.URem(zi); break;
              default: CRAB_ERROR("apron operation not supported");
            }
            set(x, xi);
          }
        }

        void backward_assign (variable_t x, linear_expression_t e,
			      apron_domain_t invariant) {
          crab::CrabStats::count (getDomainName() + ".count.backward_assign");
          crab::ScopedCrabStats __st__(getDomainName() + ".backward_assign");

          if(is_bottom()) return;

          ap_texpr0_t* t = expr2texpr (e);
          assert (t);
          auto dim_x = get_var_dim_insert (x);

	  m_var_map = merge_var_map (m_var_map, m_apstate,
				     invariant.m_var_map,
				     invariant.m_apstate);
	  
          m_apstate = apPtr (get_man (), 
                             ap_abstract0_substitute_texpr(get_man (), false, 
							   &*m_apstate, 
							   dim_x, t,
							   &*invariant.m_apstate));
                                                       
          ap_texpr0_free (t);
          CRAB_LOG("apron",
                   crab::outs() << "--- "<< x<< " :=_bwd " << e << " --> "
		                << *this<<"\n";);
        }
          
        void backward_apply (operation_t op,
			     variable_t x, variable_t y, Number z,
			     apron_domain_t invariant) {
          crab::CrabStats::count (getDomainName() + ".count.backward_apply");
          crab::ScopedCrabStats __st__(getDomainName() + ".backward_apply");

          if(is_bottom()) return;

          ap_texpr0_t* a = var2texpr (y);
          ap_texpr0_t* b = num2texpr (z);
          ap_texpr0_t* res = nullptr;
          
          switch (op){
            case OP_ADDITION: res = ADD (a, b); break;
            case OP_SUBTRACTION: res = SUB (a, b); break;
            case OP_MULTIPLICATION: res = MUL (a, b); break;
            case OP_DIVISION: res = DIV (a, b); break;
            default: CRAB_ERROR("apron operation not supported");
          }
          assert (res);

          auto dim_x = get_var_dim_insert (x);

	  m_var_map = merge_var_map (m_var_map, m_apstate,
				     invariant.m_var_map,
				     invariant.m_apstate);
	  
          m_apstate = apPtr (get_man (),
			     ap_abstract0_substitute_texpr(get_man (), false, 
							   &*m_apstate, 
							   dim_x, res,
							   &*invariant.m_apstate));
	  
          ap_texpr0_free (res);
          CRAB_LOG("apron",
                   crab::outs() << "--- "<< x << " :=_bwd " << y << op << z
		                << " --> " << *this<<"\n";);
        }
        
        void backward_apply(operation_t op,
			    variable_t x, variable_t y, variable_t z,
			    apron_domain_t invariant)  {
          crab::CrabStats::count (getDomainName() + ".count.backward_apply");
          crab::ScopedCrabStats __st__(getDomainName() + ".backward_apply");

          if(is_bottom()) return;

          ap_texpr0_t* a = var2texpr (y);
          ap_texpr0_t* b = var2texpr (z);
          ap_texpr0_t* res = nullptr;
          
          switch (op){
            case OP_ADDITION: res = ADD (a, b); break;
            case OP_SUBTRACTION: res = SUB (a, b); break;
            case OP_MULTIPLICATION: res = MUL (a, b); break;
            case OP_DIVISION: res = DIV (a, b); break;
            default: CRAB_ERROR("apron operation not supported");
          }
          assert (res);

          auto dim_x = get_var_dim_insert (x);

	  m_var_map = merge_var_map (m_var_map, m_apstate,
				     invariant.m_var_map,
				     invariant.m_apstate);
	  
          m_apstate = apPtr (get_man (),
			     ap_abstract0_substitute_texpr(get_man (), false, 
							   &*m_apstate, 
							   dim_x, res,
							   &*invariant.m_apstate));

          ap_texpr0_free (res);
          CRAB_LOG("apron",
                   crab::outs() << "--- " << x << ":=_bwd " << y << op << z
		                << " --> "<< *this <<"\n";);
        }
        	
	interval_domain_t to_interval_domain () {
	  if (is_bottom ()) return interval_domain_t::bottom();
	  if (is_top ())    return interval_domain_t::top();	  
	  
	  interval_domain_t res;
	  for (auto &px: m_var_map.left)
	    res.set (px.first, this->operator[](px.first));
	  return res;
	}
	
        linear_constraint_system_t to_linear_constraint_system () {
          linear_constraint_system_t csts;
          if(is_bottom ())  {
            csts += F ();
          }
          else if(is_top ()) {
            csts += T ();
          }
          else {
            normalize ();

            ap_lincons0_array_t lcons_arr =
	      ap_abstract0_to_lincons_array (get_man (), &*m_apstate);
            for (unsigned i=0 ; i < lcons_arr.size; i++)
              csts += tconst2const (lcons_arr.p[i]);

            ap_lincons0_array_clear (&lcons_arr);
          }
          return csts;
        }

      void rename(const variable_vector_t &from, const variable_vector_t &to) {
	if (is_top () || is_bottom()) return;
	
	// renaming m_var_map by creating a new map 
	CRAB_LOG("apron",
		 crab::outs() << "Replacing {";
		 for (auto v: from) crab::outs() << v << ";";
		 crab::outs() << "} with ";
		 for (auto v: to) crab::outs() << v << ";";
		 crab::outs() << "}:\n";
		 crab::outs() << *this << "\n";);
	
	var_map_t new_var_map;
	for (auto kv: m_var_map.left) {
	  ptrdiff_t pos = std::distance(from.begin(),
				 std::find(from.begin(), from.end(), kv.first));
	  if (pos < (int) from.size()) {
	    new_var_map.insert(binding_t(to[pos], kv.second));
	  } else {
	    new_var_map.insert(binding_t(kv.first, kv.second));
	  }
	}
	std::swap(m_var_map, new_var_map);

	CRAB_LOG("apron",
		 crab::outs () << "RESULT=" << *this << "\n");
      }
	
        void expand (variable_t x, variable_t dup) {
	  if (is_bottom() || is_top()) return;
	  
          if (get_var_dim(dup)) {
	    CRAB_ERROR("expand second parameter ", dup,
		       " cannot be already a variable in the apron domain ", *this);
	  }

          // --- increases number of dimensions by one
          auto dim_x = get_var_dim_insert (x);
          m_apstate = apPtr (get_man(),
                             ap_abstract0_expand(get_man (), false, &* m_apstate, 
                                                 dim_x, 1));
	  
          // --- the additional dimension is put at the end of integer
          //     dimensions.
          m_var_map.insert (binding_t (dup, get_dims () - 1));
        }

        void normalize () {
          ap_abstract0_canonicalize (get_man (), &*m_apstate);
        }

        void write(crab_os& o) {
          if(is_bottom()){
            o << "_|_";
            return;
          }
          else if (is_top()){
            o << "{}";
            return;
          }
          else {
            // dump ();
            linear_constraint_system_t inv = to_linear_constraint_system ();
            o << inv;
          }
        }          

        static std::string getDomainName () {
          switch (ApronDom) {
            case APRON_INT:     return "ApronIntervals"; 
            case APRON_OCT:     return "ApronOctagon"; 
            case APRON_OPT_OCT: return "ApronOptimizedOctagon"; 
            case APRON_PK:      return "ApronNewPolka";
            default: CRAB_ERROR("Unknown apron domain");
          }
        }
      }; 

      #if 1
      // Without copy-on-write
      template<class Number, class VariableName, apron_domain_id_t ApronDom>
      using apron_domain = apron_domain_<Number,VariableName,ApronDom>;     
      #else 
      // Quick wrapper which uses shared references with copy-on-write.
      template<class Number, class VariableName, apron_domain_id_t ApronDom>
      class apron_domain :
       public abstract_domain<Number, VariableName,
			      apron_domain<Number,VariableName,ApronDom> > {

        typedef apron_domain<Number, VariableName, ApronDom> apron_domain_t;
	typedef abstract_domain<Number, VariableName, apron_domain_t> abstract_domain_t;
      public:
	
        using typename abstract_domain_t::linear_expression_t;
        using typename abstract_domain_t::linear_constraint_t;
        using typename abstract_domain_t::linear_constraint_system_t;
        using typename abstract_domain_t::variable_t;
        using typename abstract_domain_t::number_t;
        using typename abstract_domain_t::varname_t;
        using typename abstract_domain_t::variable_vector_t;	
        typedef typename linear_constraint_t::kind_t constraint_kind_t;
        typedef interval<Number>  interval_t;

      private:
	
        typedef apron_domain_<Number, VariableName, ApronDom> apron_domain_impl_t;
        typedef std::shared_ptr<apron_domain_impl_t> apron_domain_ref_t;

        apron_domain_ref_t _ref;  
	
        apron_domain(apron_domain_ref_t ref) : _ref(ref) { }
        
        apron_domain_t create(apron_domain_impl_t&& t) {
          return std::make_shared<apron_domain_impl_t>(std::move(t));
        }
        
        void detach(void) {
          if(!_ref.unique())
	    _ref = std::make_shared<apron_domain_impl_t>(*_ref);
        }
        
       public:
        
        static apron_domain_t top() { return apron_domain(false); }
        
        static apron_domain_t bottom() { return apron_domain(true); }
        
        apron_domain(bool is_bottom = false)
            : _ref(std::make_shared<apron_domain_impl_t>(is_bottom)) { }
        
        apron_domain(const apron_domain_t& o)
            : _ref(o._ref) { }
        
        apron_domain& operator=(const apron_domain_t& o) {
	  if (this != &o) {
	    _ref = o._ref;
	  }
          return *this;
        }
        
        apron_domain_impl_t& ref(void) { return *_ref; }
	const apron_domain_impl_t& ref(void) const { return *_ref; }
        
        bool is_bottom() { return ref().is_bottom(); }
        bool is_top() { return ref().is_top(); }
        bool operator<=(apron_domain_t& o) { return ref() <= o.ref(); }
        void operator|=(apron_domain_t o) { detach(); ref() |= o.ref(); }
        apron_domain_t operator|(apron_domain_t o)
	{ return create(ref() | o.ref()); }
        apron_domain_t operator||(apron_domain_t o)
	{ return create(ref() || o.ref()); }
        apron_domain_t operator&(apron_domain_t o)
	{ return create(ref() & o.ref()); }
        apron_domain_t operator&&(apron_domain_t o)
	{ return create(ref() && o.ref()); }
        
        template<typename Thresholds>
        apron_domain_t widening_thresholds(apron_domain_t o, const Thresholds &ts) {
          return create(ref().template widening_thresholds<Thresholds>(o.ref(), ts));
        }
        
        void normalize() { detach (); ref().normalize(); }
	
        void operator+=(linear_constraint_system_t csts) {
	  detach(); ref() += csts;
	}
        void operator-=(variable_t v) {
	  detach(); ref() -= v;
	}
        interval_t operator[](variable_t x) {
	  return ref()[x];
	}
        void set(variable_t x, interval_t intv) {
	  detach(); ref().set(x, intv);
	}
        
        template<typename Range>
        void forget (Range vs) { detach(); ref().forget(vs); }
	
        void assign(variable_t x, linear_expression_t e) {
	  detach(); ref().assign(x, e);
	}
        void apply(operation_t op,
		   variable_t x, variable_t y, Number k) {
          detach(); ref().apply(op, x, y, k);
        }
        void apply(operation_t op,
		   variable_t x, variable_t y, variable_t z) {
          detach(); ref().apply(op, x, y, z);
        }	
        void apply(int_conv_operation_t op, variable_t dst, variable_t src) {
          detach(); ref().apply(op, dst, src);
        }
        void apply(bitwise_operation_t op,
		   variable_t x, variable_t y, Number k) {
          detach(); ref().apply(op, x, y, k);
        }
        void apply(bitwise_operation_t op,
		   variable_t x, variable_t y, variable_t z) {
          detach(); ref().apply(op, x, y, z);
        }
        void apply(div_operation_t op,
		   variable_t x, variable_t y, variable_t z) {
          detach(); ref().apply(op, x, y, z);
        }
        void apply(div_operation_t op,
		   variable_t x, variable_t y, Number k) {
          detach(); ref().apply(op, x, y, k);
        }
        void backward_assign(variable_t x, linear_expression_t e,
			     apron_domain_t invariant) {
	  detach(); ref().backward_assign(x, e, invariant.ref());
	}
        void backward_apply(operation_t op,
			    variable_t x, variable_t y, Number k,
			    apron_domain_t invariant) {
          detach(); ref().backward_apply(op, x, y, k, invariant.ref());
        }
        void backward_apply(operation_t op,
			    variable_t x, variable_t y, variable_t z,
			    apron_domain_t invariant) {
          detach(); ref().backward_apply(op, x, y, z, invariant.ref());
        }	
        void expand (variable_t x, variable_t y) {
	  detach(); ref().expand(x, y);
	}

	void rename(const variable_vector_t &from, const variable_vector_t &to)
	{ detach(); ref().rename(from, to); }
	
        template<typename Range>
        void project (Range vs) { detach(); ref().project(vs); }
        
        template <typename NumDomain>
        void push (const variable_t& x, NumDomain&inv){
	  detach(); ref().push(x, inv);
	}
        
        void write(crab_os& o) { ref().write(o); }
        
        linear_constraint_system_t to_linear_constraint_system () {
          return ref().to_linear_constraint_system();
        }
        static std::string getDomainName () {
	  return apron_domain_impl_t::getDomainName();
	}

      };
      #endif

      // -- domain traits
      template<typename Number, typename VariableName, apron_domain_id_t ApronDom>
      class domain_traits <apron_domain<Number, VariableName, ApronDom> > {
       public:
        
        typedef apron_domain<Number, VariableName, ApronDom> apron_domain_t;
        typedef ikos::variable<Number, VariableName> variable_t;
	
        template<class CFG>
        static void do_initialization(CFG cfg) { }

        static void normalize(apron_domain_t& inv) { 
          inv.normalize ();
        }
        
        template <typename Iter>
        static void forget(apron_domain_t& inv, Iter it, Iter end) {
          inv.forget (boost::make_iterator_range (it, end));
        }
        
        template <typename Iter >
        static void project(apron_domain_t& inv, Iter it, Iter end) {
          inv.project (boost::make_iterator_range (it, end));
        }
        
        static void expand(apron_domain_t& inv, variable_t x, variable_t new_x) {
          inv.expand (x, new_x);
        }		
      };
     
      // --- global datastructures
      template<typename N, typename V, apron_domain_id_t D>
      ap_manager_t* apron_domain_<N,V,D>::m_apman = nullptr;

   
   } // namespace domains
}// namespace crab
#endif 
