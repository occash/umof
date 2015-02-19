newoption
{
    trigger = 'with-tests',
    description = 'Build uMOF with unit tests',
    value = 'bool',
    allowed = { 
        {'true', 'True'} ,
        {'false', 'False'}
    }
}

newoption
{
    trigger = 'with-benchmark',
    description = 'Build uMOF with benchmars',
    value = 'bool',
    allowed = { 
        {'true', 'True'} ,
        {'false', 'False'}
    }
}

solution 'metasystem'
	configurations { 'Release', 'Debug' }
	location 'build'
	
	project 'umof'
		targetname 'umof'
		language 'C++'
		kind 'SharedLib'
		
		files
		{
			'src/**.h',
			'src/**.cpp'
		}
		
		configuration 'Debug'
			targetdir 'bin/debug'
			defines { '_DEBUG', 'UMOF_LIBRARY' }
			flags { 'Symbols' }
			
		configuration 'Release'
			targetdir 'bin/release'
			defines { 'NDEBUG', 'UMOF_LIBRARY' }
			flags { 'Optimize' }
			
		configuration 'gmake'
			buildoptions { '-std=c++11' }

    if _OPTIONS['with-tests'] == 'true' then
        project 'test'
            targetname 'umof-test'
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
			defines { '_DEBUG', 'UMOF_LIBRARY' }
			flags { 'Symbols' }
			
            configuration 'Release'
                targetdir 'bin/release'
                defines { 'NDEBUG', 'UMOF_LIBRARY' }
                flags { 'Optimize' }
                
            configuration 'gmake'
                buildoptions { '-std=c++11' }
            
    end
    
    if _OPTIONS['with-benchmark'] == 'true' then
        project 'benchmark'
            targetname 'umof-benchmark'
            language 'C++'
            kind 'ConsoleApp'
            
            files
            {
                'test/bench/**.h',
                'test/bench/**.cpp'
            }
            
            links { 'umof' }
            includedirs { 'src' }
            
            configuration 'Debug'
			targetdir 'bin/debug'
			defines { '_DEBUG', 'UMOF_LIBRARY' }
			flags { 'Symbols' }
			
            configuration 'Release'
                targetdir 'bin/release'
                defines { 'NDEBUG', 'UMOF_LIBRARY' }
                flags { 'Optimize' }
                
            configuration 'gmake'
                buildoptions { '-std=c++11' }
            
    end
    