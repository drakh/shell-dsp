#include <vector>

#include <ladspa.h>
#include <dssi.h>

#include "kicker.hh"

// // namespace shell
// {
//   class DssiDescriptor
//   {
//   public:
//     /* ladspa stuff */
//     LADSPA_Descriptor *ladspaDescriptor() const;

//     /* dssi stuff */
//     DSSI_Descriptor *dssiDescriptor() const;

//   private:
//     LADSPA_Descriptor ladspa_desc_;
//     DSSI_Descriptor   dssi_desc_;
//   };
// }

// static char *dssi_configure(LADSPA_Handle  ctx,
//                             const char    *key,
//                             const char    *value)
// {
//   return nullptr;
// }

// static const LADSPA_Descriptor g_ladspa_descriptors[] = {
//   {
//     19042, // UniqueID
//     "shell-kicker", // Label
//     {}, // properties
//     "Shell Kicker", // Name
//     "Alexandre BIQUE", // Maker
//     "None", // Copyright
//     0, // PortCount
//     {}, // PortDescriptors
//     {}, // PortNames
//     {}, // PortRangesHints
//     NULL, // ImplementationData
    
//   },
// };

// static const DSSI_Descriptor g_dssi_descriptors[] = {
//   {
//     1, // api version
//     g_ladspa_descriptors + 0,
    
//   },
// };

// const DSSI_Descriptor *dssi_descriptor(unsigned long index)
// {
//   static std::vector<shell::Dssi> plugins;

//   if (index < plugins.size())
//     return plugins[i].dssiDescriptor();
//   return nullptr;
// }
