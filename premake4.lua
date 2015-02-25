newoption
{
    trigger = 'with-tests',
    description = 'Build with unit tests',
}

newoption
{
    trigger = 'with-benchmark',
    description = 'Build with benchmars',
}

newoption
{
    trigger = 'static',
    description = 'Build static library'
}

solution 'metasystem'
	configurations { 'Release', 'Debug' }
	location 'build'
	
	project 'umof'
		targetname 'umof'
		language 'C++'
        
        if _OPTIONS['static'] then
            kind 'StaticLib'
            defines { 'UMOF_STATIC' }
        else
            kind 'SharedLib'
        end
		
		files
		{
			'src/**.h',
			'src/**.cpp'
		}
		
		defines { 'UMOF_LIBRARY' }
		
		configuration 'Debug'
			targetdir 'bin/debug'
			defines { '_DEBUG' }
			flags { 'Symbols' }
			
		configuration 'Release'
			targetdir 'bin/release'
			defines { 'NDEBUG' }
			flags { 'Optimize' }
			
		configuration 'gmake'
			buildoptions { '-std=c++11' }

    if _OPTIONS['with-tests'] then
        project 'test'
            targetname 'umof-tests'
            language 'C++'
            kind 'ConsoleApp'
            
            files
            {
                'test/unit/**.h',
                'test/unit/**.cpp'
            }
            
            links { 'umof' }
            includedirs { 'src' }
            
            configuration 'Debug'
				targetdir 'bin/debug'
				defines { '_DEBUG' }
				flags { 'Symbols' }
			
            configuration 'Release'
                targetdir 'bin/release'
                defines { 'NDEBUG' }
                flags { 'Optimize' }
                
            configuration 'gmake'
                buildoptions { '-std=c++11' }
            
    end
    
    if _OPTIONS['with-benchmark'] then
        project 'benchmark'
            targetname 'umof-benchmark'
            language 'C++'
            kind 'ConsoleApp'
            
            files { 'test/bench/**.cpp' }
            links { 'umof', 'cpgf' }
            includedirs { 'src' }
            
            configuration 'Debug'
				links { 'celerod', 'campd', 'qt5cored' }
				targetdir 'bin/debug'
				defines { '_DEBUG' }
				flags { 'Symbols' }
			
            configuration 'Release'
				links { 'celero', 'camp', 'qt5core' }
                targetdir 'bin/release'
                defines { 'NDEBUG' }
                flags { 'Optimize' }
                
            configuration 'gmake'
                buildoptions { '-std=c++11' }
    end
    