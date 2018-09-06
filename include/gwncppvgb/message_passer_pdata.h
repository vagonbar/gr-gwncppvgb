/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */


#ifndef INCLUDED_GWNCPPVGB_MESSAGE_PASSER_PDATA_H
#define INCLUDED_GWNCPPVGB_MESSAGE_PASSER_PDATA_H

#include <gwncppvgb/api.h>

#include <string>

namespace gr {
  namespace gwncppvgb {

    /*!
     * \brief <+description+>
     *
     */


	//void process_data(std::string ev);

    class GWNCPPVGB_API message_passer_pdata
    {
    public:
      message_passer_pdata(std::string ev);
      ~message_passer_pdata();
	  static std::string process_data(std::string ev);
    private:
    };

  } // namespace gwncppvgb
} // namespace gr

#endif /* INCLUDED_GWNCPPVGB_MESSAGE_PASSER_PDATA_H */

