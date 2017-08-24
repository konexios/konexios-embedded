#ifndef RELOC_INSTANCE_INTEGRITY_H
#define RELOC_INSTANCE_INTEGRITY_H

#include "reloc_assert.h"

#ifndef SKIP_INTEGRITY_CHECK
#define INSTANCE_INTEGRITY_SET( this_ )     {                                       \
                                                ASSERT( this_ != NULL);             \
                                                this_->self = this_;                \
                                            }
                                            
#define INSTANCE_INTEGRITY_CHECK( this_ )   {                                       \
                                                ASSERT( this_ != NULL);             \
                                                ASSERT( this_->self == this_ );     \
                                            }

#define INSTANCE_INTEGRITY_DESTROY( this_ ) {                                       \
                                                ASSERT( this_ != NULL);             \
                                                this_->self = NULL;                 \
                                            }

#else

#define INSTANCE_INTEGRITY_SET( this_ )
#define INSTANCE_INTEGRITY_CHECK( this_ )
#define INSTANCE_INTEGRITY_DESTROY( this_ )

#endif

#endif
