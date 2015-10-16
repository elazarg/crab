#ifndef CRAB_SPARSE_GRAPH_HPP
#define CRAB_SPARSE_GRAPH_HPP
#include <vector>
#include <crab/common/types.hpp>

/* A (slightly) cleaner implementation of a sparse weighted graph.
 * 
 */
namespace ikos {

template <class Weight>
class SparseWtGraph : public writeable {
  public:
    typedef Weight Wt;
    typedef SparseWtGraph<Wt> graph_t;

    typedef unsigned int vert_id;

    SparseWtGraph(unsigned int _sz = 10, float _growth_rate = 1.4)
      : max_sz(_sz), sz(_sz), growth_rate(_growth_rate),
        fwd_adjs((unsigned int*) malloc(sizeof(unsigned int)*max_sz*(max_sz+1))),
        rev_adjs((unsigned int*) malloc(sizeof(unsigned int)*max_sz*(max_sz+1))),
        mtx((Wt*) malloc(sizeof(Wt)*max_sz*max_sz))
    {
      for(vert_id v = 0 ; v < sz; v++)
      {
        succs(v).clear(); preds(v).clear();
      }
    }
        
    template<class Wo>
    SparseWtGraph(SparseWtGraph<Wo>& o)
      : max_sz(o.max_sz), sz(o.sz), growth_rate(o.growth_rate),
        fwd_adjs((unsigned int*) malloc(sizeof(unsigned int)*max_sz*(max_sz+1))),
        rev_adjs((unsigned int*) malloc(sizeof(unsigned int)*max_sz*(max_sz+1))),
        mtx((Wt*) malloc(sizeof(Wt)*max_sz*max_sz))
    {
      for(vert_id v = 0 ; v < sz; v++)
      {
        succs(v).clear(); preds(v).clear();
      }

      /*
      for(auto e : o.edges())
      {
        add_edge(e.src, (Wt) e.weight, e.dest); 
      }
      */
      for(vert_id v = 0; v < o.size(); v++)
        for(vert_id d : o.succs(v))
          add_edge(v, o.edge_val(v, d), d);
    }

    ~SparseWtGraph()
    {
      for(vert_id v = 0; v < sz; v++)
      {
        for(vert_id d : succs(v))
          (&(mtx[max_sz*v + d]))->~Wt();
      }
      free(mtx);
      free(fwd_adjs);
      free(rev_adjs);
    }

    vert_id new_vertex(void)
    {
      vert_id v;
      if(free_id.size() > 0)
      {
        v = free_id.back();
        free_id.pop_back();
      } else {
        vert_id v = sz++;
        if(max_sz <= sz)
        {
          growCap(max_sz*growth_rate);
        }
      }
      preds(v).clear();
      succs(v).clear();
      return v;
    }

    void forget(vert_id v)
    {
      // FIXME: currently assumes v is not free
      free_id.push_back(v); 
      
      // Remove (s -> v) from preds.
      for(vert_id s : succs(v))
      {
        (&(mtx[max_sz*v + s]))->~Wt();
        preds(s).remove(v);
      }
      succs(v).clear();

      // Remove (v -> p) from succs
      for(vert_id p : preds(v))
      {
        (&(mtx[max_sz*p + v]))->~Wt();
        succs(p).remove(v); 
      }
      preds(v).clear();
    }

    // Check whether an edge is live
    bool elem(vert_id x, vert_id y) {
      return succs(x).mem(y);
    }

    Wt& edge_val(vert_id x, vert_id y) const {
      return mtx[max_sz*x + y];
    }

    // Precondition: elem(x, y) is true.
    Wt operator()(vert_id x, vert_id y) const {
      return mtx[max_sz*x + y];
    }

    void clear_edges(void) {
      for(vert_id v : verts())
      {
        // Make sure the matrix entries are destructed
        for(vert_id d : succs(v))
          (&(mtx[max_sz*v + d]))->~Wt();

        preds(v).clear();
        succs(v).clear();
      }
    }

    // Number of allocated vertices
    int size(void) const {
      return sz;
    }

    // Assumption: (x, y) not in mtx
    void add_edge(vert_id x, Wt wt, vert_id y)
    {
      assert(!elem(x, y));
      succs(x).add(y);
      preds(y).add(x);
      // Allocate a new Wt at the given offset.
      new (&(mtx[x*max_sz + y])) Wt(wt);
    }

    void set_edge(vert_id s, Wt w, vert_id d)
    {
      if(!elem(s, d))
        add_edge(s, w, d);
      else
        edge_val(s, d) = w;
    }

    template<class Op>
    void update_edge(vert_id s, Wt w, vert_id d, Op& op)
    {
      if(elem(s, d))
      {
        edge_val(s, d) = op(edge_val(s, d), w);
        return;
      }

      if(!op.default_is_absorbing())
        add_edge(s, w, d);
    }

    class vert_iterator {
    public:
      vert_iterator(vert_id _v)
        : v(_v)
      { }
      vert_id operator*(void) const { return v; }
      vert_iterator& operator++(void) { ++v; return *this; }
      bool operator!=(const vert_iterator& o) const { return v != o.v; }
    protected:
      vert_id v;
    };
    class vert_list {
    public:
      vert_list(vert_id _sz)
        : sz(_sz)
      { }
      vert_iterator begin(void) const { return vert_iterator(0); } 
      vert_iterator end(void) const { return vert_iterator(sz); }
      unsigned int size(void) const { return (unsigned int) sz; }
    protected:
      vert_id sz; 
    };
    vert_list verts(void) const { return vert_list(sz); }

    class adj_list {
    public:
      adj_list(unsigned int* _ptr, unsigned int max_sz)
        : ptr(_ptr), sparseptr(_ptr+1+max_sz)
      { }
      vert_id* begin(void) const { return (vert_id*) (ptr+1); } 
      vert_id* end(void) const { return (vert_id*) (ptr+1+size()); }
      vert_id operator[](unsigned int idx) const { return ptr[1+idx]; }
      unsigned int* sparse(void) const { return sparseptr; }
      vert_id* dense(void) const { return ptr + 1; }
      unsigned int size(void) const { return *ptr; }

      bool mem(unsigned int v) const {
        unsigned int idx = sparse()[v];
        return idx < size() && dense()[idx] == v;
      }
      void add(unsigned int v) {
        assert(!mem(v));
        unsigned int idx = *ptr;
        dense()[idx] = v;
        sparse()[v] = idx;
        (*ptr)++;
      }
      void clear() { *ptr = 0; }

    protected:
      unsigned int* ptr;
      unsigned int* sparseptr;
    };

    adj_list succs(vert_id v)
    {
      return adj_list(fwd_adjs + v*(2*max_sz+1), max_sz);
    }
    adj_list preds(vert_id v)
    {
      return adj_list(rev_adjs + v*(2*max_sz+1), max_sz);
    }
  protected:
    // Allocate new memory, and duplicate 
    // the content.
    // Add an element 
    void _adj_add(unsigned int* adj, unsigned int max, unsigned int val)
    {
      adj[1 + *adj] = val;
      adj[1 + max + val] = *adj;
      (*adj)++;
    }

    void growCap(unsigned int new_max)
    {
      if(new_max <= max_sz)
        return;

      unsigned int new_mtxsz = sizeof(Wt)*new_max*new_max;
      unsigned int new_adjsz = sizeof(unsigned int)*(2*new_max+1);

      Wt* new_mtx = malloc(new_mtxsz);
      unsigned int* new_fwd = (unsigned int*) malloc(new_adjsz);
      unsigned int* new_rev = (unsigned int*) malloc(new_adjsz);

      for(vert_id v = 0; v < sz; v++)
      {
        unsigned int* new_fwd_ptr = new_fwd + v*new_adjsz; 
        *new_fwd_ptr = 0;
        for(vert_id d : succs(v))
        {
          _adj_add(new_fwd_ptr, new_max, d);
          new_mtx[v*new_max + d] = mtx[v*max_sz + d];
        }

        unsigned int* new_rev_ptr = new_rev + v*new_adjsz;
        *new_rev_ptr = 0;
        for(vert_id s : preds(v))
          _adj_add(new_rev_ptr, new_max, s);
      }
      free(mtx); 
      free(fwd_adjs);
      free(rev_adjs);

      mtx = new_mtx;
      fwd_adjs = new_fwd;
      rev_adjs = new_rev;
    }
       
    void write(std::ostream& o) {
      o << "[|";
      bool first = true;
      for(vert_id v = 0; v < sz; v++)
      {
        auto it = succs(v).begin();
        auto end = succs(v).end();

        if(it != end)
        {
          if(first)
            first = false;
          else
            o << ", ";

          o << "[v" << v << " -> ";
          o << "(" << edge_val(v, *it) << ":" << *it << ")";
          for(++it; it != end; ++it)
          {
            o << ", (" << edge_val(v, *it) << ":" << *it << ")";
          }
          o << "]";
        }
      }
      o << "|]";
    }

    unsigned int max_sz;
    unsigned int sz;
    float growth_rate;

    // Each element of fwd/rev adjs:
    // [ sz/1 | dense/max_sz | inv/max_sz ]
    // Total size: sizeof(uint) * max_sz * (1 + 2*max_sz)
    unsigned int* fwd_adjs;
    unsigned int* rev_adjs;
    Wt* mtx;

    std::vector<int> free_id;
};

}
#endif
