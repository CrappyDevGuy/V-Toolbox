//_Author : CrappyDevGuy, M12D08Y2020_//

#pragma once

namespace vt
{

	class instance
	{

		public:
			instance();
			instance(const instance&) = delete;
			~instance();

			instance& operator=(const instance&) = delete;

			instance& operator=(instance&& other) noexcept
			{
				return *this;
			}		

			instance(instance&& other) noexcept
			{
				
			}

		private:
			
	};

}
