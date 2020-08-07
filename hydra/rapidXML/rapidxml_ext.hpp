//
//  rapidxml_ext.hpp
//  rapid_test
//
//  Created by Michael Callahan on 6/26/20.
//  Copyright © 2020 Michael Callahan. All rights reserved.
//

#ifndef rapidxml_ext_h
#define rapidxml_ext_h

#pragma once

// Adding declarations to make it compatible with gcc 4.7 and greater
// See https://stackoverflow.com/a/55408678
namespace rapidxml {
    namespace internal {
        template <class OutIt, class Ch>
        inline OutIt print_children(OutIt out, const xml_node<Ch>* node, int flags, int indent);

        template <class OutIt, class Ch>
        inline OutIt print_attributes(OutIt out, const xml_node<Ch>* node, int flags);

        template <class OutIt, class Ch>
        inline OutIt print_data_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);

        template <class OutIt, class Ch>
        inline OutIt print_cdata_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);

        template <class OutIt, class Ch>
        inline OutIt print_element_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);

        template <class OutIt, class Ch>
        inline OutIt print_declaration_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);

        template <class OutIt, class Ch>
        inline OutIt print_comment_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);

        template <class OutIt, class Ch>
        inline OutIt print_doctype_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);

        template <class OutIt, class Ch>
        inline OutIt print_pi_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);
    }
}

#endif /* rapidxml_ext_h */
