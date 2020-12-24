# libdatetimetools
A library of tools for performing date and time calculations.

# Description
Description: libdatetimetools is a library of useful functions to enable
users to perform date and time calculations. One of its primary uses is as a
component of solutions to computing chain-based scheduling, as in litigation
and project management. 

# Version info
Version: 1.0
Created: 12/19/2020 12:43:04
Last Modified: Wed Dec 23 16:01:48 2020


#Usage:
Static library.

#File Formats:

Presently the library usese CSV files to import various rules
such as holidays.

# Restrictions:
Presently, the library handles Unix encoded ASCII files fine,
but chokes on macOS and Windows files. Handling CR and CRLFs will be
addressed in the future.

#Error Handling:
Under development

# License & Disclaimer
SPDX-License-Identifier:  LGPL-3.0-only

Author: Thomas H. Vidal (THV), thomashvidal@gmail.com
Organization: Dark Matter Computing

Copyright: (c) 2011-2020 - Thomas H. Vidal, Los Angeles, CA
All rights reserved

libdatetimetools is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by the
Free Software Foundation, version 3 of the License.
 
libdatetimetools is distributed in the hope that it will be useful,but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
details.

You should have received a copy of the GNU General Public License along with
libdatetimetools.  If not, see <https://www.gnu.org/licenses/>.
