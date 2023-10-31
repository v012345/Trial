TitleScence = {}
function TitleScence:init()
    self.sKeyOfPackagelib = nil
    for keyOfPackagelib, packagelib in pairs(package.loaded) do
        if packagelib == self then
            self.sKeyOfPackagelib = keyOfPackagelib
            break
        end
    end
    if not self.sKeyOfPackagelib then
        error("can't find packagelib : TitleScence")
    end
end

function TitleScence:update()

end

function TitleScence:destroy()
    package.loaded[self.sKeyOfPackagelib] = nil
end
